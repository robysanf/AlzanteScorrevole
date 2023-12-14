void controlla_tasca() {
  if ( _Dir == 0) {                      
    return;
  }
  if ( _Dir == 1) {                                                                                // -- SE STO APRENDO
    for (int i = 1 ; i < Numero_Ante_Slave ; i++) {
      if ((Pos_Anta[i] - Pos_Anta[i + 1]) < (pos_aperto[i] - pos_aperto[i + 1] - 100 * imp)) {     // -- se sono vicino a sbattere contro l'anta
         if (Direzione_Anta[i+1] != 0 ){                                                           // -- se non è già partita devo far partire l'anta che segue 
           attendi_risposta( i, 10, "2224");
    delay(1);                                                        
         }
      }
    }
  }
  if ( _Dir == -1) {                                  // -- SE STO CHIUDENDO
                                                      // -- CREDO NON CI SIA NULLA DA CONTROLLARE
  }
}

void partenza_tasca(int direzione) {
  // -- se  tutti su

  int avvio_anta[Numero_Ante_Slave];            // -- MI SERVE PER RICORDARE CHE HO DELLE ANTE DA FAR PARTIRE
  if  (Stato_Anta[1] == "T") {
    // -- 1 TUTTI SU
  }
  if (direzione == 1) {
    int riavvio = 0;
    for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {     // -- CONTROLLE CHE LE ANTE NON SIANO TROPPO VICINE
      // -- se la prima anta non ha spazio la metto in lista per partire
      if ( (i < Numero_Ante_Slave) && (Pos_Anta[i] - Pos_Anta[i + 1]) < (pos_aperto[i] - pos_aperto[i + 1] - 200 * imp)) {
        avvio_anta[i] = true;
        riavvio++;
      }
      else {
        // -- la faccio partire con velocità max
        // -- fare su slave WARD_TASCA();
        break;
      }

    }
    if (riavvio > 0) {
      for (int i = 1 ; i <= riavvio ; i++) {
        if (avvio_anta[i] == true) {
          // -- la faccio partire
        }
      }
    }
  }
  if (direzione == -1) {
    int riavvio = 0;
    for (int i = Numero_Ante_Slave ; i >= 1 ; i--) {
      // -- se la prima anta non ha spazio la metto in lista per partire
      if (abs(Pos_Anta[i] - Pos_Anta[i - 1]) > abs(pos_aperto[i] - pos_aperto[i - 1] - 200 * imp)) {
        avvio_anta[i] = true;
        riavvio++;
      }
      else {
        // -- la faccio partire
      }

    }
    if (riavvio > 0) {
      for (int i = 1 ; i <= riavvio ; i++) {
        if (avvio_anta[i] == true) {
          // -- la faccio partire
        }
      }
    }
  }
}
