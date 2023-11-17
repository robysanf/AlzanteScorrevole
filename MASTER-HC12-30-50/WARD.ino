void ferma_WARD() {
  int i;
  for ( i = 1; i <= Numero_Ante_Slave  ; i = i + 1) {
    inVia(i, "2221",33); delay(10);
    attendi_risposta(i, 5, "2221");
    solopassaggio = false;
    apertura = false;
    _Dir = 0;
    delay(1);
  }
}

void fast_ward() {
  int i;
  for ( i = 1; i <= Numero_Ante_Slave  ; i = i + 1) {
    inVia(i, "2225",32);
    attendi_risposta(i, 5, "2225");
    Serial.print("fast,"); Serial.print(i); Serial.println(",18");
    delay(1);
  }
}
