void ascolta_web() {
    if ( millis() - time_strwebHostOld > 2500){
      strwebHostOld = "0000";
    }
    delay(wait);
    if (strwebHost != strwebHostOld ) {
    strweb = strwebHost.substring(0, 4);
    strweb2 = strwebHost.substring(4);
   
    if ( strweb == "chch") {                    // -- comando per FERMARE
      
      Serial.println("cambia canale");
       for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
        inVia(i, "5555"+strweb2,45); 
        attendi_risposta(i, 10, "5555");
        strwebHostOld = strwebHost;
      }
      digitalWrite(3, LOW );
      delay(150);
      String s="AT+C"+strweb2;
      char lis[255];
      s.toCharArray(lis,255);
      Serial1.write(lis);
      delay(150);
      digitalWrite(3, HIGH );
    }
    delay(1);
    strwebHost = "0000";
    delay(1);
  }
}
