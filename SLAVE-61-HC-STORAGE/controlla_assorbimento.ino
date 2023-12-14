void controlla_assorbimento() {
  if ( digitalRead(A7) == LOW || Stato_Alzata[0] != 'U') {
    Alza_Anta();
  }
  digitalWrite(4, HIGH);                                    // ABILITA I MOTORI
  if (Stato_Anta[0] = 'T') {                                // SE TUTTO CHIUSO APRO
    _Dir = 1;
  }
  else if (Stato_Anta[0] = 'A') {                           // SE TUTTO APERTO CHIUDO
    _Dir = -1;
  }
  else {
    return;                                                 // altrimenti non finziona
  }
  int i = 0;
  int limiteora;
  velocita_crocera = velocita_crocera_MAX;
  for ( i = 70; i <= TopSpeed; i = i + 1) {
    md.setM2Speed(i * motore * _Dir);
    tensione = i + 1;
    limiteora = md.getM2CurrentMilliamps();
    if (limiteora > 3500) {
      Serial.print("in accellerazione controlla_assorbimento limite  di " + String(limiteora) + " superato");
      ferma_WARD(30);
      return;
    }
    if  (crocera(1)) {
      Serial.print("RAGGIUNTA VELOCITA' CROCERA , TENSIONE = "); Serial.println(tensione);
      break;
    }
    Ascolta_Master();
    if (str == 2221 || str == 2223 ) {
      Serial.println("azione stop annulla test controlla_assorbimento");
      ferma_WARD(7);
      str = 0;
      return;
    }
    delay(5);
  }
  while ((pos < pos_aperto - spazioRallenta && _Dir == 1) || (pos > spazioRallenta && _Dir == -1)) {
    int passo = (pos_aperto - (spazioRallenta * 2)) / 100;
    for (int i = 0 ; i <= 99; i++) {
      while (pos < i * passo + spazioRallenta) {
        Ascolta_Master();
        if (str == 2221 || str == 2223 ) {
          ferma_WARD(7); str = 0; return;
          delay(10);
        }
        limiteora = md.getM2CurrentMilliamps();
        if (limiteora > 3500) {
          Serial.println("in controlla_assorbimento limite  di " + String(limiteora) + " superato");
          ferma_WARD(30);
          return;
        }
        array_assorbimento[i] = fai_media();
      }

    }
  }
  Serial.println("fine controlla_assorbimento");
  ferma_WARD(7);
}

void stampa_assorbimento(){
  Serial.println("stampo gli assorbimenti:");
  for( int i =0; i<=99;i++){
    Serial.println("   consumo settore " +String(i) + " = " + String(array_assorbimento[i]));
  }
}
