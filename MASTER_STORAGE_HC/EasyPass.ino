void check_easyPass() {
  if (Pos_Anta[1] > easy_pass && Stato_Anta[1] != "F" ) {
    ferma_WARD();
    solopassaggio = false;
    return;
  }
}

void reset() {
  ferma_WARD();
  return;
}

void check_posizione() {
  Per_Centuale = pos_aperto[1] / 100;
  if ( _percentuale != 0 && (Pos_Anta[1] / Per_Centuale) > _percentuale && _Dir == 1) {    // -- PER APPLICAZIONE FUTURA CON SWIPE
    ferma_WARD(); 
    _percentuale = 0;
  }
  if ( _percentuale != 0 && (Pos_Anta[1] / Per_Centuale) < _percentuale && _Dir == -1) {
    ferma_WARD();  // -- PER APPLICAZIONE FUTURA CON SWIPE
    _percentuale = 0;
  }

  if (Stato_Anta[1] == "T" && dipendente == true ) {    // -- dico al PRINCIPALE che puo andare
    digitalWrite(CONSENSO_in_scrittura, LOW); 
  }
  if (Stato_Anta[1] != "T" && dipendente == true ) {    // -- dico al PRINCIPALE di stare fermo
    digitalWrite(CONSENSO_in_scrittura, HIGH); 
  }

  if (Stato_Anta[1] == "T" && dipendente == false ) {   // -- dico al DIPENDENTE di stare fermo
    digitalWrite(CONSENSO_in_scrittura, HIGH); 
  }
  if (Pos_Anta[1] >= 3000 && dipendente == false ) {
    digitalWrite(CONSENSO_in_scrittura, LOW);           // -- dico al DIPENDENTE che puo andare
  }

  if (Pos_Anta[1] <= 3000 && _Dir == -1 && digitalRead(CONSENSO_in_lettura) == HIGH) {                  // -- NON HO IL CONSENSO A CHIUDRE E QUINDI FERMO E ASPETTO
    ferma_WARD();
    int cont = 0;
    while (digitalRead(CONSENSO_in_lettura) == HIGH) {                                                  // -- "HIGH" qui vuol dire NO STAI FERMO
      ascolta_web();
      if (comando != "0000" || digitalRead(STOP) == LOW || digitalRead(APRI) == LOW || cont > 3000 ) {  // -- fermo tutto
        strweb = "";
        return;
      }
      delay(50);
      cont++;
    }
    Partenza_Sicura(-1, 0, 0);                          // -- se il tempo non è scaduto  e nessuno ha fermato allora quando ho il permesso vado in chiusura    
  }
}
