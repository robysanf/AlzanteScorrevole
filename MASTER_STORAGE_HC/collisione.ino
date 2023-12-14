
void controlla_collisione() {
  if (_Dir == 1) {//apri
    int i ;
    for ( i = 1; i < Numero_Ante_Slave  ; i = i + 1) {
      collisione_[i] = (collisione_anta_[i] - Pos_Anta[i] + Pos_Anta[i + 1]);
      if ( Stato_Anta[i] == "V" && collisione_[i] < Spazio_Collisione) {
        Serial.print("DIR = ");Serial.println(_Dir);
        fast_ward(); return;
      }
    }
  }
  if (_Dir == -1) {//chiudi
    int i ;
    for ( i = 1; i < Numero_Ante_Slave  ; i = i + 1) {
      collisione_[i] = (Pos_Anta[i] - Pos_Anta[i + 1]);
      if ( Stato_Anta[i] == "V" && collisione_[i] < (Spazio_Collisione)) {
         Serial.print("DIR = ");Serial.println(_Dir);
        fast_ward(); return;
      }
    }
  }
}
