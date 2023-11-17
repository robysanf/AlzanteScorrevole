void chiedo_configurazioni() {
  Serial.println("inizio chiedo_configurazioni");
  imp = dataRead("Imp").toFloat();
  Serial.println("Imp = " + String(imp));
  Distanza = Distanza_init * imp;                              // convertito in impulsi
  easy_pass = easy_pass_init * imp;                            // convertito in impulsi
  Spazio_Partenza = Spazio_Partenza_init * imp;                // convertito in impulsi
  spazioRallenta = spazioRallenta_init * imp;                  // convertito in impulsi
  Spazio_Collisione = Spazio_Collisione_init * imp;            // convertito in impulsi
  Spazio_Collisione_hold = Spazio_Collisione;
  spazio_no_chk_vel = spazio_no_chk_vel_init * imp;

  if (dataRead("Dipendente") == "false") {
    dipendente = false;                                                    // - altrimenti rimane true
  }
  Serial.print("dipendente = "); Serial.println(dipendente);
  digitalWrite(CONSENSO_in_scrittura, HIGH );                              // -- digitalWrite(CONSENSO_in_scrittura , (dipendente ? HIGH : LOW));ù

  String controllo;
  delay(100);
  String temporaneo_addr = scanner();
  boolean check = false;
  // -- RICEVO E SETTO LE VARIABILI
  controllo = dataRead("configurazione");                                  // -- legge da datainit il valore di configurazione per sapere se fare il
  Serial.println("Ricevi2 = " + controllo);
  Serial.print("temporaneo_addr ="); Serial.println(temporaneo_addr);

  parseVar2( NUMERO_ANTE_SLAVE , temporaneo_addr);
  int Numero_Ante_Slave_File = dataRead("Numero_Ante_Slave").toInt();
  Serial.print(" Numero_Ante_Slave ="); Serial.println(Numero_Ante_Slave);
  Serial.print(" Numero_Ante_Slave file = "); Serial.println(Numero_Ante_Slave_File);
  if ( controllo == "true" && (Numero_Ante_Slave == Numero_Ante_Slave_File)) {



    parseVar2( ANTE_SLAVE , dataRead("Ante_Slave"));                     // --

    parseVar2( ADDR , temporaneo_addr);                                  // -- mi trovo anche il numero degli slave presenti
    int conta_ante = 0;
    // @-- confronta le ante lette da file e quelle lette da scanner numero ante slave deve essere sostituito da temporaneo.length nel ciclo interno


    for (int i = 1; i <= Numero_Ante_Slave; i = i + 1) {                 // @-- Serial.print("\n addr[ *9***");
      for (int b = 1; b <= Numero_Ante_Slave; b = b + 1) {
        // @-- Serial.print("\n addr[ ");Serial.print(b);Serial.print("] = ");Serial.print(String(addr[b]));
        // @-- Serial.print("\n Ante_Slave[ ");Serial.print(i);Serial.print("] = ");Serial.print(String(Ante_Slave[i]));
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
    parseVar2( ANTE_SLAVE , temporaneo_addr);
    for (int c = 1; c <= Numero_Ante_Slave; c++)
    {
      Serial.print(c); Serial.print(" - "); Serial.println(Ante_Slave[c]);
    }

  }
  Serial.print("  NUMERO_ANTE_SLAVE = "); Serial.println(Numero_Ante_Slave);
}

void parseVar2(nomeVar clr, String varList) {

  // @-- Serial.print("Splitting string into tokens: "); Serial.println(String(varList));
  char list[70];
  varList.toCharArray(list, 70);
  char * pch;

  pch = strtok (list, ",");
  int cont = 1;
  //int o = 0;
  while (pch != NULL)
  {
    switch (clr)
    {
      case DIPENDENTE:               dipendente = String(pch);                                         // @-- Serial.print("dipendente = "); Serial.println(dipendente); break;
      case NUMERO_ANTE_SLAVE:        Numero_Ante_Slave = String(pch).toInt(); Numero_Ante_Slave = cont; break;
      case VELOCITA_CROCERA_MAX:     velocita_crocera_MAX = String(pch).toFloat(); break;
      case VELOCITA_CROCERA_LENTA:   velocita_crocera_LENTA = String(pch).toFloat(); break;
      case ANTE_SLAVE:               Ante_Slave[cont] = String(pch).toInt(); ; break;                   // @-- Serial.print(" Ante_Slave pch: "); Serial.println(Ante_Slave[cont])
      case RITARDO:                  ritardo[cont] = String(pch).toFloat(); break;
      case ATTRITO:                  attrito[cont] = String(pch); break;                                // @-- Serial.print("\n attrito pch: "); Serial.print(attrito[cont]);
      case MOTORE:                   motore[cont] = String(pch); break;
      case ENCODER:                  encoder[cont] = String(pch); break;
      case INERZIA:                  inerzia[cont] = String(pch).toInt(); break;
      case PESO:                     peso[cont] = String(pch).toInt(); break;
      case ADDR:                     addr[cont] = String(pch).toInt(); break;                           // @-- Serial.print("\n addr= "); Serial.print(String( addr[cont]));
      case POS_APERTO:               pos_aperto[cont] = String(pch).toInt(); break;
      // etc
      default: break;
    }
    cont++;
    pch = strtok (NULL, ",");
  }
}
