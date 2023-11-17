boolean button_pressed() {
  if (riavvio_slave_avvenuto == true) {
    return false;
  }
  if ( (digitalRead(CHIUDI) == LOW || digitalRead(APRI) == LOW || digitalRead(STOP) == LOW) && set == 0 ) {
    return true;
  }
  return false;
}
//*******************************************
//   CALCOLA VELOCITA'
//*******************************************
void calcola_velocita() {
  int temporaneo;
  String temporaneo2;
  float costante;                            // -- RAPPRESENTA LA DIFFERENZA DI VELOCITA' TRA LE ANTE
  //char _temporaneo[32];
  int i;
  if (Numero_Ante_Slave > 1) {
    costante = (velocita_crocera_MAX - velocita_crocera_LENTA) / (Numero_Ante_Slave - 1);
  }
  else {
    costante = 0;
  }
  Serial.print("\n costante = "); Serial.println(costante);
  for (i = 1; i <= Numero_Ante_Slave; i = i + 1) {
    Serial.print("\n costante i = "); Serial.println(i);
    if (ritardo[i] == NULL) {
      ritardo[i] = 1;
      Serial.print("\n costante i2 = ");
      Serial.println(i);
    }
    Serial.println("\n maxspeed = " + String(velocita_crocera_MAX));
    velocita_crocera_MAX_slave_[i] = velocita_crocera_MAX - ((i - 1) * costante) * ritardo[i] ;
    temporaneo = velocita_crocera_MAX_slave_[i] * 100;
    temporaneo2 = "3331" + String(temporaneo);
    inVia(i, temporaneo2, 31);
    attendi_risposta(i, 10, "3331");
    //delay(10);
  //  inVia(i, "0000", 30);
    Serial.print("\n velocita_crocera_MAX_slave_["); Serial.print(i); Serial.print("] = "); Serial.println(velocita_crocera_MAX_slave_[i]);
  }
  Serial.println("Calcolo velocita conclusa = " + String(velocita_crocera_LENTA));
  Serial.println("Spazio_Collisione default = "); Serial.print(Spazio_Collisione_hold);
  Serial.println("maxspeed/40 = " + String(velocita_crocera_MAX / 40.00));
  Serial.println("Spazio_Collisione calcolo = "); Serial.print(Spazio_Collisione);
}

//*******************************************
//   SET_RESET_RIDOTTO
//*******************************************
void set_reset_ridotto() {
  if (dipendente == false) {
    Serial.println("principale alza");
    for ( int i = 1; i <= Numero_Ante_Slave   ; i = i + 1) {
      Serial.print("fermo tutto "); Serial.println(i);
      inVia(i, "2221", 129);
      attendi_risposta(i, 10, "2221");
      delay(500);
      //ricEvi();
    }
    delay(1000);
    for ( int i = 1; i <= Numero_Ante_Slave   ; i = i + 1) {
      Serial.print("ORDINO ALZA ANTA "); Serial.println(i);
      inVia(i, "1119", 29);
      attendi_risposta(i, 10, "1119");
      delay(500);
    }
    for ( int i = 1; i <= Numero_Ante_Slave   ; i = i + 1) {

      Serial.print( "aspetto alza anta "); Serial.println(i);
      int cont = 0;
      while (true && cont < 1000) {
        inVia(i, "0000", 28);
        //delay(150);
        //Serial.print( "Cerco stato alzata ");
        ricEvi();
        cont++;
        if (Stato_Alzata[i] == "U") {
          Serial.print( "trovato U ");
          break;
        }
     }
      delay(150);
    }
    digitalWrite(CONSENSO_in_scrittura, LOW);
  }

  if (check_dipendente(30000) == false) { // -- devo aspettare che il principale sia alto
    return;
  }

  boolean fatto = false;
  for ( int i = 1; i <= Numero_Ante_Slave   ; i = i + 1) {
    inVia(i, "1111", 27);
    attendi_risposta(i, 10, "1111");
    Serial.print(" ORDINO ALLA ANTA "); Serial.print(i); Serial.println(" DI CHIUDERE");
    delay(300);
  }
  Serial.println(" ASPETTO STATO ");
 /* for (int i = 0; i < 20; i++) {
    ricEvi();
    delay(5);
    //Serial.println(" ASPETTO STATO " + String(i));
  }*/
  delay(5);
  //ricEvi();
  int cont_r = 0;
  while (true) {                 // -- aspetto che chiudano tutte le ante
    delay(20);
   // ricEvi();
    if (cont_r > 1500) {
      return;
    }
    for ( int i = 1; i <= Numero_Ante_Slave   ; i = i + 1) {
      inVia(i, "0000", 25);
      delay(10);
      ricEvi();
      Serial.print(" stato anta"); Serial.print(i); Serial.println(" - "); Serial.println(Stato_Anta[i]);
      if (Stato_Anta[i] != "R") {
        // -- SE tutte le ante sono ferme esco
        break;
      }

      if (i == Numero_Ante_Slave) {
        fatto = true;
      }
    }
    cont_r++;
    delay(50);
    if (fatto == true) {
      fatto = false;
      break;
    }
  }
  // -- se arrivo qua ordino di fare il giochetto con la ginghia setp
  for ( int i = 1; i <= Numero_Ante_Slave   ; i = i + 1) {
    inVia(i, "1115", 24);
    attendi_risposta(i, 10, "1115");
    Serial.print( "memorizzo il pos_chiuso su anta "); Serial.println(i);
    int cont_O = 0;
    while (true) {
       inVia(i, "0000", 23);
      if (cont_O > 100) {
        fatto = false;
        return;
      }
      //Serial.print( "Cerco O ");
      
      ricEvi();
      //delay(150);
      if (Stato_Anta[i] == "O") {
        Serial.print( "trovato O ");
        fatto = true;
        break;
      }
      cont_O ++;
     
    }
    delay(150);
  }
  if (fatto == false) {
    return; // se non trovo tutti gli o stoppo il set reset
  }
  // -- ORA DEVO CALCOLARE ANCHE IL VALORE DI COLLISIONE:
  // -- LE COLLISIONI SONO >> Numero_Ante_Slave - 1 ;
  // -- collisione_[i] = pos_aperto[1] - pos_aperto[i+1]

  // -- IL FOR CHE SEGUE NON è CORRETTO
  fatto = false;
  for ( int i = 1; i < Numero_Ante_Slave   ; i = i + 1) {
    collisione_anta_[i] = pos_aperto[i] - pos_aperto[i + 1];
    Serial.print("\n collisione_anta_"); Serial.print(i); Serial.print(" = "); Serial.print(collisione_anta_[i]);
    delay(1);
  }
  for ( int i2 = 1; i2 <= Numero_Ante_Slave  ; i2++) { //.print("finito il set anche per");Serial.println(i2);
    inVia(i2, "1116", 22);
    attendi_risposta(i2, 10, "1116");
    delay(200);
  }
  for ( int i2 = 1; i2 <= Numero_Ante_Slave  ; i2++) { //.print("finito il set anche per");Serial.println(i2);
    inVia(i2, "2222", 21);
    attendi_risposta(i2, 10, "2222");
    delay(200);
  }
  int cont = 0;
  while (true) {
    if (cont > 100) {
        fatto = false;
        return;
      }
    cont++;
    Serial.print( "Cerco T ");
    delay(150); 
    inVia(1, "0000", 20);
    ricEvi();
    if (Stato_Anta[1] == "T") {
      Serial.print( "trovato T ");
      fatto = true;
      break;
    }
   
  }
  Serial.print( "sono qui");
  delay(100);
  digitalWrite(CONSENSO_in_scrittura , (dipendente ? LOW : HIGH));
  if (dipendente) {
    int c = 0;
    Serial.print("CONSENSO_in_lettura == "); Serial.print(digitalRead(CONSENSO_in_lettura));
    while (digitalRead(CONSENSO_in_lettura) == HIGH && c < 14000) { // -- aspetto che il principale abbia finito
      Serial.println( "aspetto che il principale finisca il set_reset");
      c++;
      delay(100);
    }
    if (c >= 15000) {
      return;
    }
  }
  set = 1 ;
  _Dir = 0 ;
  Serial.println( "FINITO il set_reset");
  delay(2000);
}
