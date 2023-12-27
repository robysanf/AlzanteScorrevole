/****************************************************************************************************************************
  WiFIUdpWebServer.ino
  For boards with WiFiNINA module/shield.

  created 25 Nov 2012
  by Roberto Seghetto e Roberto Sanfelici
  adapted to WiFi AP Example

  Version: 1.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0
*****************************************************************************************************************************/

#include "defines.h"
#include "arduino_secrets.h"
#include "Variabili.h"
#include <WiFiWebServer.h>


#define BUFFER_SIZE     5000

extern "C" char* sbrk(int incr);


WiFiWebServer server(80);

// *************************************** please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;              // your network SSID (name)
char pass[] = SECRET_PASS;              // your network password (use for WPA, or use as key for WEP), length must be 8+
unsigned int localPort = 2390;          // local port to listen on
int test_alzata = 0;
int cicli_test = 0;
long parti = 0;
boolean test_chiudi = true;
int led =  LED_BUILTIN;
char packetBuffer[256];                 //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";   // a string to send back
String hold_packet[10];

int status = WL_IDLE_STATUS;
void setup()
{
  Comando[1] = "0000";
  Serial.begin(9600);
  Serial1.begin(9600);//per modulo HC12
  delay(500);
  pinMode(CONSENSO_in_lettura, INPUT_PULLUP);             // 16 in caso di due mega (intrecciare)
  pinMode(CONSENSO_in_scrittura, OUTPUT);                 // 17 in caso di due mega (intrecciare)
  digitalWrite(CONSENSO_in_scrittura, HIGH );             // nego il consenso a procedere di default
  pinMode(APRI, INPUT_PULLUP);
  pinMode(CHIUDI, INPUT_PULLUP);
  pinMode(STOP, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH );
  Serial.println("Start AP_SimpleWebServer on " + String(BOARD_NAME));
  pinMode(led, OUTPUT);                                   // set the LED pin mode
  if (WiFi.status() == WL_NO_MODULE)                      // check for the WiFi module:
  {
    Serial.println("Communication with WiFi module failed!");
    testAp();
    // while (true);                                         // don't continue
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }



  Serial.print("Creating access point named: "); Serial.println(ssid);
  WiFi.config(ip); // ************************************** commentare questa riga se si definisce l'IP nel route statico del Mango
  status = WiFi.beginAP(ssid, pass);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.print(F("HTTP server started @ "));
  Serial.println(WiFi.localIP());
  delay(1000);
  //Storage_Load();
}

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void loop() {
  serialEvent();
  if (stringComplete) {
    if (inputString == "APRI") {
      Partenza_Sicura(1, 0, 0);
    Serial.println("APRI DA SERIAL");
    }
    if (inputString == "CHIUDI") {
      Partenza_Sicura(-1, 0, 0);
    Serial.println("CHIUDI DA SERIAL");
    }
    if (inputString == "STOP") {
      ferma_WARD();
    Serial.println("STOP DA SERIAL");
    }
    inputString = "";
    stringComplete = false;
  }
  //long tempo = micros();
  server.handleClient();                                             // -- controllo il web
  delay(1);
  ascolta_web();                                                     // -- controllo il web
  if ( riavvio_slave_avvenuto ) {                                    // -- rifaccio il set_reset_ridotto(); se uno slave si riavvia
    riavvio_slave_avvenuto = false;
    set = 0;
    chiedo_conf = true;
  }
  if ( chiedo_conf ) {                                               // -- al primo giro di vita parto con il setup se già impostato altrimenti entro in loop con set==0
    chiedo_configurazioni();
    chiedo_conf = !chiedo_conf;
  }
  if ( apertura && Pos_Anta[1] > mm_apri_fisso.toInt()*imp) {        // -- in caso di più ante faccio partire tutti dopo l'apri fisso
    Partenza_Sicura(1, 0, 0);
    apertura = false;
  }
  if ( uno_giu  && check_alzata(false)) {                            // -- faccio partire tutti se uno slave stava giù
    Partenza_Sicura(1, 0, 0);
    uno_giu = false;
  }
  Check_Pin();                                              // -- controllo i pulsanti
  if (stringComplete) {
    Serial.println(inputString);
    Dime = inputString;
    // clear the string:
    inputString = "";
    stringComplete = false;
  } else {
    Dime = "0000";
  }
  delay(1);
  // -- controllo le ante e se il comando mi ritorna                                                            // -- controllo le ante e se il comando mi ritorna
  delay(1);
  check_posizione();                                                     // -- controllo dipendente
  delay(1);
  // -- controllo easyPass
  delay(1);
  controlla_collisione();                                                // -- controllo collisione
  delay(1);
  //controlla_tasca();                                                   // -- controllo tasca
  delay(1);
  if (conta > 120 && set == 0) {
    //  chiedo_conf = true;                           
    conta = 0;
  }
  for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
    inVia(i, Dime, 1);
    ricEvi();
    if (emergenza) {
      Emergenza(emergenza_chi, emergenza_dir, emergenza_buffer);
      Serial.println("emergenza");
      emergenza = false;
      emergenza_chi = 0;
      emergenza_dir = 0;
      emergenza_buffer = "";
    }
  }
  conta++;
  //Serial.print("Tempo = "); Serial.println(micros() - tempo);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
      break;
    }
    inputString += inChar;
  }
}
