void Abbassa_Anta() {
  //ferma_WARD(7);
  digitalWrite(4, HIGH);
  Serial.println("ABBASSO ANTA");
  boolean fuori = false;
  Serial.print("alzo digitalRfead(A7) = "); Serial.println(digitalRead(A7));
  Serial.print("alzo digitalRead(A6) = "); Serial.println(digitalRead(A6));
  if (digitalRead(A7) == HIGH) {
    long timer = 1;
    int consumo;
    for ( int i = 1; i <= 350; i = i + 1) {
      if (digitalRead(A7) == LOW) {
        md.setM1Speed(0);
        Serial.println("breK ANTA");
        break;
      }
      consumo = md.getM1CurrentMilliamps();
      if (consumo >= 1000) { //Serial.print(" CONSUMO D1 exit = ");Serial.println(consumo);
        md.setM1Speed(0); fuori = true; break;
      }
      md.setM1Speed(i);
      delay(1);
    }
    while (digitalRead(A7) != LOW && timer <= 10000 && !fuori) {
      if ((timer % 100) == 0) {
        md.setM2Speed(-450 * motore);
      }
      if ((timer % 200) == 0) {
        md.setM2Speed(0);
      }
      consumo = md.getM1CurrentMilliamps();
      // Serial.print(timer);Serial.print(" CONSUMO D2 = ");Serial.println(consumo);
      if (consumo >= 1000) { //Serial.print(" CONSUMO D3 exit = ");Serial.println(consumo);
        md.setM1Speed(0); break;
      }
      delay(1);
      timer ++;
    }
    Serial.println("END ANTA");
    md.setM1Speed(0);
    md.setM2Speed(300);
    delay(300);
    md.setM2Speed(0);

  }
  Stato_Alzata[0] = 'D';
  digitalWrite(4, LOW);
  return;
}

void Alza_Anta() {
  digitalWrite(4, HIGH);
  Serial.println("ALZO ANTA");
  boolean fuori = false;
  if (Stato_Alzata[0] != 'U' || digitalRead(A6) == HIGH) {
    Stato_Alzata[0] = 'M';
    long timer = 1;
    int consumo;
    for ( int i = 40; i <= 400; i = i + 1) { 
      if (digitalRead(A6) == LOW) {
        md.setM1Speed(0);
        Stato_Alzata[0] = 'U';
        Serial.println("breK ANTA");
        break;
      }
      md.setM1Speed(-i);
      delay(2);
      consumo = md.getM1CurrentMilliamps();
      if (consumo >= 2000) {
        // @-- Serial.print(" CONSUMO U1 exit = "); Serial.println(consumo);
        md.setM1Speed(0); fuori = true;
        break;
      }
    }
    while ( digitalRead(A6) != LOW && timer <= 8000 && !fuori) {
      consumo = md.getM1CurrentMilliamps();
      // @-- Serial.print(timer); Serial.print(" CONSUMO U2 = "); Serial.println(consumo);
      if (consumo >= 2000) {
        Serial.print(" CONSUMO U3 exit = "); Serial.println(consumo);
        md.setM1Speed(0);
        break;
      }
      Ascolta_Master();

      // -----------------------------------


      if (str == 2221) {
        si_stop_sblocco = true;

      }

      // -------------------------------------

      delay(5);
      timer ++;
    }
    md.setM1Speed(0);
  }
   if (digitalRead(A6) == LOW) {
        md.setM1Speed(0);
        Stato_Alzata[0] = 'U';
   }
        
  digitalWrite(4, LOW);
  return;
}
