void testAp() {
  String da_scrivere = "";
  for (int i = 0; i < 3; i++) {
    WiFi.end();
    server.stop();
    if (WiFi.status() == WL_NO_MODULE)                      // check for the WiFi module:
    {
      Serial.println("Communication with WiFi module failed!");
      continue;
    }
    Serial.print("Creating access point named: "); Serial.println(ssid);
    WiFi.config(ip); // ************************************** commentare questa riga se si definisce l'IP nel route statico del Mango
    server.begin();
    status = WiFi.beginAP(ssid, pass);
    if (status != WL_AP_LISTENING)
    {
      continue;
    } else
    {
      break;
    }
  }
}
