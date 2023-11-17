void testAp(){
    String da_scrivere ="";
  for(int i=0;i<3;i++){
  WiFi.end();
 
  server.stop();
  //status = WL_IDLE_STATUS;
  Serial.println("ReStart AP_SimpleWebServer on ");
  da_scrivere = "APERTURA WIFI = "+i ;
  da_scrivere = da_scrivere + " ** " + String(millis()/60000);
  writelog(da_scrivere);
  if (WiFi.status() == WL_NO_MODULE)                      // check for the WiFi module:
  {
    Serial.println("Communication with WiFi module failed!");
    da_scrivere = da_scrivere +"Communication with WiFi module failed!";
    writelog(da_scrivere);
    continue;
  }
 
  Serial.print("Creating access point named: ");Serial.println(ssid);
  WiFi.config(ip); // ************************************** commentare questa riga se si definisce l'IP nel route statico del Mango
 server.begin();
   
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING)
  {
    Serial.println("Creating access point failed");

    da_scrivere = da_scrivere +"Creating access point failed";
    writelog(da_scrivere);

    continue;
  }else
  {break;}
  }//for
  }
