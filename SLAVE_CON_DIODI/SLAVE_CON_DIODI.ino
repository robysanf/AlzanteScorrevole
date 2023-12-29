//***************************************************
#include "defines.h"
#include "arduino_secrets.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//*************************************************

#include "DualMC33926MotorShield.h"
#include "VARIABILI.h"
#define _Aprimi_DEBUG

DualMC33926MotorShield md;

extern "C" char* sbrk(int incr);

//************************************************************************
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP), length must be 8+
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
unsigned int localPort = 2300 + atoi(sonoio); //61;      // local port to listen on
char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[255];       // a string to send back
char hold_packet[255];
IPAddress ip(192, 168, 81, atoi(sonoio)); //si usa se non ho il router che gli assegna lui IP
IPAddress ipdns(192, 168, 8, 1);
IPAddress ipgate(192, 168, 8, 1);
IPAddress ipmask(255, 255, 255, 0);

//**********************************************************************

int led =  LED_BUILTIN;//pin 13 su 33 IOT

void setup() {
  pinMode(5, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(led, OUTPUT);       // set the LED pin mode
  pinMode(A2, INPUT);         // VALORE DEL PIN PER CONTROLLO TENSIONE
  pinMode(A7, INPUT_PULLUP);  // VALORE DEL PIN PER ANTA FINE CORSA BASSO
  pinMode(A6, INPUT_PULLUP);  // VALORE DEL PIN PER ANTA FINE CORSA BASSO
  pinMode(2, INPUT_PULLUP);  //PIN ENCODER
  pinMode(3, INPUT_PULLUP);  //PIN ENCODER
  digitalWrite(5, HIGH);
  digitalWrite(11, HIGH);
  attachInterrupt(digitalPinToInterrupt(3), avanti_3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(2), avanti_2, RISING);
  //***************************** INIZIALIZZO MOTORI
  md.init();
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(2000);
  Serial.println("Init Motore");
  digitalWrite(4, HIGH);
  delay(2);
  md.setM2Speed(0);
  digitalWrite(4, LOW);
  //***********
  //*********** moltiplico per imp perchè la variabile in variabili.h indica i millimetri e check pos controlla il pos cioè il numero di impulsi
  //***********
  spazioRallenta = spazioRallenta * imp;
  quasiChiuso = quasiChiuso * imp;
  spazio_no_chk_vel = spazio_no_chk_vel * imp;
  Stato_Alzata[0] = 'I';
  //************ IMPOSTAZIONI WIFI
  //************ SE SI USA ROUTING STATICO SUL ROUTER TOGLIERE COMMENTARE WIFI.CONFIG
  Serial.println("\nStart Client WiFiUdpSendReceiveString on " + String(BOARD_NAME));
  if (WiFi.status() == WL_NO_MODULE)                                       // check for the WiFi module
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
   // while (true);
  }
  String fv = WiFi.firmwareVersion();
  WiFi.config(ip);//,ipdns,ipgate,ipmask);//COMMENTARE SE ROUTING STATICO
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }
  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

 /* while (status != WL_CONNECTED)                                            // attempt to connect to Wifi network:
  {
    Serial.print("setup Attempting to connect to SSID: "); Serial.println(ssid);
    status = WiFi.begin(ssid, pass);                                        // Connect to WPA/WPA2 network. Change this line if using open or WEP network
    delay(500);                                                             // wait 5 seconds for connectio
    conteggio ++;
  }*/
  Serial.println("Connected to wifi");
  printWifiStatus();
  Serial.println("\nStarting connection to server...");
 
  //*******************************************************************
  // ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);
}

void loop() {
  long tempo=micros();
/*  while (WiFi.status() != WL_CONNECTED) {
    ferma_WARD(7);
    Udp.stop();
    WiFi.end();
    WiFi.config(ip);
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(500);
    Udp.begin(localPort);
    conteggio ++;
    Serial.println("conteggio");
  }*/
  //********* RICEVE I DATI DAL MASTER E INVIA LA STRINGA DI RISPOSTA CON LO STATO ALZATA, IL DIR, IL POS, E EVENTUALE EMERGENZA
  Ascolta_Master();
  delay(5);
  //********** INTERPRETA LA STRINGA RICEVUTA ED ESEGUE IL RELATIVO COMANDO
  che_faccio();
  fai_media();
  //********** STAMPA LO STATO OGNI 50 GIRI

  delay(5);
  if (_Dir != 0) {
    controlla_velocita_WARD();
    // -----------------------------------------------
    // PASSO DA qua  e guardo il tempo
    // se il tempo salvato in passato è piu vecchio di un secondo
    if ( (millis() - check_time) > 1000 ) {
      //   entro  controllo il movimento e salvo il tempo e il pos
      check_time = millis();
      int pos_dif = abs(check_pos_old - pos);
      if ( pos_dif < 5 * imp ) { //  vuol dire 0,5 cm/sec
        Serial.println("fermo per niente");
        Serial.print(" pos_dif = "); Serial.println(pos_dif);
        ferma_WARD(30);
        //devo avvisare il master che ho fermato qui
        Stato_Anta[0] = 'S'; 
        // fatto
      }
      check_pos_old = pos;
    }
    // -----------------------------------------------
  }
  if (_Dir == 0) {
    for (int ic = 0; ic < 20; ic++) {
      corrente[ic] = 0;
    }
  }
  if (set == 1)
  {
    check_pos();
  }
  delay(5);
  if (  _Dir != 0 && limit()) {
    emergenza(66);
  }
  delay(5);
  tensione_batteria = map(analogRead(A2), 0, 4095, 0, 3300);
  contatore = contatore + 1 ;
  Serial.print("Tempo = ");Serial.println(micros()-tempo);

  if (_Dir == 0) {
    if ( ( (micros() - T1) > (1 / (15 * imp)) * 1000000 * (cadenza * 2)) && V_M != 0) { // -- se sono fermo e son passat1 (1/(15*imp))*1000000*(cadenza*2)) microsecondi allora consi
      V_M = 0;
      Serial.print("V_M 00 = "); Serial.println(V_M);
    }
    // -- SE DA FERMO MUOVO L'ANTA STAMPO IL POS ****
    if (pos_vecchio_loop != pos ) {
      Serial.print("pos = "); Serial.println(pos);
      Serial.print("V_M = "); Serial.println(V_M);
      Serial.print("consumo = "); Serial.println(md.getM2CurrentMilliamps());
    }
  }
  pos_vecchio_loop = pos;

  
}


void printWifiStatus()
{
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.print("WIFI STATUS: "); Serial.println(WiFi.status());
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}

void stampaACapo(String s) {
  stampa(s + "\n\r");
}
void stampa(String s)
{
#ifdef _Aprimi_DEBUG
  Serial.print(s);
#endif
}


void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
