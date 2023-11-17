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
#include <SD.h>

#define BUFFER_SIZE     5000

extern "C" char* sbrk(int incr);

File root;
WiFiWebServer server(80);
File fsUploadFile;
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
  //delay(3000);
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
  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
  Serial.print("Creating access point named: "); Serial.println(ssid);
  WiFi.config(ip); // ************************************** commentare questa riga se si definisce l'IP nel route statico del Mango
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING)
  {
    Serial.println("Creating access point failed");
    testAp();
    //while (true);
  }
  server.on(F("/"), handleRoot);
  server.on(F("/test.svg"), drawGraph);
  server.on(F("/inline"), []()
  {
    server.send(200, F("text/plain"), F("This works as well"));
  });
  server.on("/uploadf", HTTP_POST,                       // if the client posts to the upload page
  []() {},
  handleFileUpload                                       // Receive and save the file
           );
  server.on("/upload", HTTP_GET, []() {                  // if the client requests the upload page
    if (!handleFileRead("/upload.htm"))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found");  // otherwise, respond with a 404 (Not Found) error
  });
  // ------------------------------------------------------
  server.on("/pippo", HTTP_GET, []() {                  // if the client requests the upload page
    if (!handleFileRead("/pippo.htm"))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  // -------------------------------------------------------
  server.onNotFound(handleNotFound);
  server.begin();
  printWiFiStatus();                                    // you're connected now, so print out the status
  
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    //while (1);                                          // don't continue
  }
  Serial.println("initialization done.");
  writelog("initialization done.");
  Serial.print(F("HTTP server started @ "));
  Serial.println(WiFi.localIP());
  root = SD.open("/");
  printDirectory(root, 0);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000);
  if (!SD.exists("49.txt")) {
    File f = SD.open("49.txt", FILE_WRITE | O_CREAT);
    f.print("0");
    f.close();
  }
  Ante_Slave[1]=61;
  Ante_Slave[2]=62;
  Ante_Slave[3]=63;
  Numero_Ante_Slave = 3;
}

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void loop() {
  serialEvent();
  //long tempo = micros();
  server.handleClient();                      // -- controllo il web
  delay(1);
  if ( riavvio_slave_avvenuto ) {              // -- rifaccio il set_reset_ridotto(); se uno slave si riavvia
    riavvio_slave_avvenuto = false;
    set = 0;
    chiedo_conf = true;
  }
  if ( chiedo_conf ) {                        // -- al primo giro di vita parto con il setup se già impostato altrimenti entro in loop con set==0
    chiedo_configurazioni();
    chiedo_conf = !chiedo_conf;
    writelog("Configurazione conclusa");
  }
  if ( apertura && Pos_Anta[1] > mm_apri_fisso.toInt()*imp) {            // -- in caso di più ante faccio partire tutti dopo l'apri fisso
    Partenza_Sicura(1, 0, 0);
    apertura = false;
  }
  if ( uno_giu  && check_alzata(false)) {                                // -- faccio partire tutti se uno slave stava giù
    Partenza_Sicura(1, 0, 0);
    uno_giu = false;
  }

 /* for (int i = 1; i <= Numero_Ante_Slave; i++) {
    if (not_packet[i] > 100 && set == 1) {
      not_packet[i] = 0;
      testAp(i);
    }
  }*/
 
  //conteggio++;
  Check_Pin(); // -- controllo i pulsant
 
  if (stringComplete) {
    Serial.println(inputString);
    Dime=inputString;
    // clear the string:
    inputString = "";
    stringComplete = false;
  }else{
    Dime="0000";
    }
  delay(1);
  //ricEvi();                                                              // -- controllo le ante e se il comando mi ritorna                                                            // -- controllo le ante e se il comando mi ritorna
  delay(1);
  check_posizione();                                                     // -- controllo dipendente
  delay(1);
  //check_easyPass();                                                      // -- controllo easyPass
  delay(1);
  controlla_collisione();                                                // -- controllo collisione
  delay(1);
  //controlla_tasca();                                                   // -- controllo tasca
  
  ascolta_web();                                                         // -- controllo il web
  delay(1);
  //Dime = "0000"+String(conta);
  if (conta > 120 && set == 0) {
  //  chiedo_conf = true;
    conta = 0;
    //Dime = "1234";
  }
  
 for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
    //Serial.print("no_packet[" +String(i) + "] = "); Serial.println(not_packet[i]);
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
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar == '\n') {
      stringComplete = true;
      break;
      }
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    
      
    
  }
}
