void testDurata() {
  if (!test) {
    return;
  }
  if (parti == 0) {
    cicli_test = cicli_test + 1;
    Serial.print("\n Nuovo Ciclo***************************************** DI " + String(cicli_test) + " effettuati");
    parti = millis();
    Partenza_Sicura(1, 0, 0); Serial.print("\n APRI");
  }
  long now = millis();
  if (now - parti < 35000 ) {
    return;
  }
  if (test_chiudi) {
    Partenza_Sicura(-1, 0, 0); Serial.print("\n CHIUDI"); 
    test_chiudi = false;
  }
  else {
    if (now - parti > 70000) {
      parti = 0;
      test_alzata = 0;
      test_chiudi = true;
    } else {
      return;
    }
  }
}

/*char* getLogTime() {
  char lifeTime[]={'0','0',':','0','0',':','0','0',':','0','0'};
  if (!log_time) {
    return lifeTime;
  }
   int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;
  
  hr = hr % 24;
  
  lifeTime[1]=day+'0';
  lifeTime[4]=hr+'0';
  lifeTime[7]=min+'0';
}*/
