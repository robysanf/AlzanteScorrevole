void chiedo_configurazioni() {
  Serial.println("inizio chiedo_configurazioni");
  Storage_Load();
  Serial.println("Imp = " + String(imp));
  Distanza = Distanza_init * imp;                              // convertito in impulsi
  easy_pass = easy_pass_init * imp;                            // convertito in impulsi
  Spazio_Partenza = Spazio_Partenza_init * imp;                // convertito in impulsi
  spazioRallenta = spazioRallenta_init * imp;                  // convertito in impulsi
  Spazio_Collisione = Spazio_Collisione_init * imp;            // convertito in impulsi
  Spazio_Collisione_hold = Spazio_Collisione;
  spazio_no_chk_vel = spazio_no_chk_vel_init * imp;

  Serial.print("dipendente = "); Serial.println(dipendente);
  digitalWrite(CONSENSO_in_scrittura, HIGH );                              // -- digitalWrite(CONSENSO_in_scrittura , (dipendente ? HIGH : LOW));

  String controllo;
  delay(100);
  int Numero_Ante_Slave_File = Numero_Ante_Slave;
  String temporaneo_addr = scanner();
  stringaToIntArray(temporaneo_addr,Ante_Slave);
  boolean check = false;
  // -- RICEVO E SETTO LE VARIABILI
  controllo = configurazione;                                  // -- legge da datainit il valore di configurazione per sapere se fare il
  Serial.println("Ricevi2 = " + controllo);
  Serial.print("temporaneo_addr ="); Serial.println(temporaneo_addr);
  Numero_Ante_Slave=calcolaAnte(temporaneo_addr);
  Serial.print(" Numero_Ante_Slave = "); Serial.println(Numero_Ante_Slave);
  Serial.print(" Numero_Ante_Slave file = "); Serial.println(Numero_Ante_Slave_File);
  if ( controllo == "true" && (Numero_Ante_Slave == Numero_Ante_Slave_File)) {                             
    stringaToIntArray(temporaneo_addr, addr);
    int conta_ante = 0;
    // @-- confronta le ante lette da file e quelle lette da scanner numero ante slave deve essere sostituito da temporaneo.length nel ciclo interno

    for (int i = 1; i <= Numero_Ante_Slave; i = i + 1) {                 // @-- Serial.print("\n addr[ *9***");
      for (int b = 1; b <= Numero_Ante_Slave; b = b + 1) {
        Serial.print("\n addr[ ");Serial.print(b);Serial.print("] = ");Serial.print(String(addr[b]));
        Serial.print("\n Ante_Slave[ ");Serial.print(i);Serial.print("] = ");Serial.print(String(Ante_Slave[i]));
        if (addr[b] == Ante_Slave[i]) {
          conta_ante = conta_ante + 1;
          break;
        }
      }
    }
    if (conta_ante == Numero_Ante_Slave) {                               // @-- se l'incrocio dei dati non funziona ordino di resettare il file dati.ini esco

      Serial.println("configurazione caricata con successo");
      check = true;
      Serial.println("Invio dati agli slave");
      delay(10);
      //**************************************************************************
      setVar();
      //**************************************************************************
      int c = 0;
      Serial.println("VADO Dentro : button_pressed()  *****");
      while (true) {
        ascolta_web();
        if ( button_pressed()) {
          return;
        }
        if ( comando != "0000") {
          comando = "0000";
          Serial.println("web....");
          return;
        }
        delay(10);
        c = c + 1;
        if ( c > 100) {
          break;
        }
      }
      Serial.println("ESCO : button_pressed()  *****");
      //calcola_velocita();
      //**********************************************************************************
      set_reset_ridotto();
      //**********************************************************************************
      //Serial.print("\n set_reset_ridotto(); ");
      // -- adesso tutto e settato correttamente
      // -- mi metto in ascolto dei pulsanti e dello yun
    }
  }
  if (!check) {
    stringaToIntArray(temporaneo_addr, Ante_Slave);
    for (int c = 1; c <= Numero_Ante_Slave; c++)
    {
      Serial.print(c); Serial.print(" - "); Serial.println(Ante_Slave[c]);
    }

  }
  Serial.print("  NUMERO_ANTE_SLAVE = "); Serial.println(Numero_Ante_Slave);
}

int calcolaAnte(String str){
   char list[50];
  
  str.toCharArray(list, 50);
  char * pch;
  pch = strtok (list, ",");
  int  cont = 0;
  while (pch != NULL)
  {
    pch = strtok (NULL, ",");
    cont++;
  }
  return cont;
  }
  
