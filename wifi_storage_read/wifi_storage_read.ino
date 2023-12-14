#include "defines.h"
#include "variabili.h"
#include <WiFiWebServer.h>
IPAddress ip(192, 168, 1, 90);
WiFiWebServer server(80);
int wifimode = 0;
char ssid[] = SECRET_SSID;              // your network SSID (name)
char pass[] = SECRET_PASS;              // your network password (use for WPA, or use as key for WEP), length must be 8+
int status = WL_IDLE_STATUS;
void setup() {
  // put your setup code here, to run once:
  WIFIMode();
  server.onNotFound(handleNotFound);
  server.begin();


}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  delay(100);
}
