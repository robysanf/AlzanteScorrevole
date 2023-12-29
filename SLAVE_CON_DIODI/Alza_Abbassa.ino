int corrente_alzo[20];
int conta_corrente_alzo = 0;
int tensione_attuale = 0;

void Alza_Abbassa(int top) {  // -- top  >> +600  -600
  digitalWrite(4, HIGH);
  int i = 0;
  if (tensione_attuale != 0) {
    i = tensione_attuale;
  }
  int direzione = 1;
  if (top < 0) {
    top = top * -1;
    direzione = -1;
  }
  if (!Finecorsa(direzione)) {  // -- vado avanti
    for (i = tensione_attuale; i <= top; i = i + 1) {
      fai_media_alzo();
      md.setM1Speed(i * direzione * -1);
      delay(1);
    }
    tensione_attuale = i;
    int conteggio = 0;
    boolean fatto = false;
    while (conteggio < 6000) {
      tensione_attuale = i;
      if (fai_media_alzo() < 50 && tensione_attuale > 0) {
        Serial.println("INIZIO FERMO");
        for (i = v_attuale; i >= 0; i = i - 50) {
          md.setM1Speed(i * direzione * -1);
          delay(1);
        }
        fatto = true;
        break;
      }
      fai_media_alzo();
      conteggio++;
      delay(1);
    }
    Serial.println("FERMO");
    if (direzione == 1) { Stato_Alzata[0] = 'U'; }
    if (direzione == -1) { Stato_Alzata[0] = 'D'; }
    if (fatto == false) { Stato_Alzata[0] = 'E'; }
  } else {
    if (direzione == 1) { Serial.println("GIA' APERTO"); }
    if (direzione == -1) { Serial.println("GIA' CHIUSO"); }
  }
}
boolean Finecorsa(int direzione) {
  long tempo = millis();
  digitalWrite(4, HIGH);
  int i = 0;
  for (i = 0; i <= 300; i = i + 5) {
    fai_media_alzo();
    tensione_attuale = i;
    md.setM1Speed(i * direzione * -1);
  }
  if (fai_media_alzo() < 50) {
    md.setM1Speed(0);
    Serial.print("Tempo = ");
    Serial.println(millis() - tempo);
    return true;
  } else {
    Serial.print("Tempo = ");
    Serial.println(millis() - tempo);
    return false;
  }
}
int fai_media_alzo() {
  conta_corrente_alzo = conta_corrente_alzo + 1;
  if (conta_corrente_alzo > 19) {
    conta_corrente_alzo = 0;
  }
  corrente_alzo[conta_corrente_alzo] = md.getM1CurrentMilliamps();
  int tot = 0;
  for (int i = 0; i < 20; i++) {
    tot = tot + corrente_alzo[i];
  }
  return tot / 20;
}

