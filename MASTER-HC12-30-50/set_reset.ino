void set_reset() {
  int i;
  boolean fatto = false;
  set = 0;
  // -- CHIUDO TUTTO:
  //-----------------------------------------------------------------------------------
  for ( i = 1; i <= Numero_Ante_Slave   ; i = i + 1)
  { //ordina("1111/",Ante_Slave[i],3);
    inVia(i, "1111",8);
    //Serial.print("ORDINO ALLA ANTA ");Serial.print(i);Serial.println(" DI CHIUDERE");
    delay(500);
  }
  //-----------------------------------------------------------------------------------

  // -- STO CHIUDENDO
  // -- Serial.print("STO CHIUDENDO : ");
  while (true) {
    inVia(i, "0000",7);
    for ( i = 1; i <= Numero_Ante_Slave   ; i = i + 1) {
      ricEvi();
      if (Stato_Anta[i] != "R") {  // -- SE tutte le ante sono ferme esco
        break;
      }
      if (i == Numero_Ante_Slave) {
        fatto = true;
      }

    }
    delay(100);

    if (button_pressed() == true) {
      ferma_WARD();
      digitalWrite (RELAY1, HIGH);
      return;
    }
    delay(1);

    if (fatto == true) {
      fatto = false;
      break;
    }
  }

  // se arrivo qua ordino di fare il giochetto con la ginghia setp

  for ( i = 1; i <= Numero_Ante_Slave   ; i = i + 1) {
    //ordina("1115/", Ante_Slave[i], 4); //Serial.print( "memorizzo il pos_chiuso su anta "); Serial.println(i);
    inVia(i, "1115",6);
    while (true) {
      ricEvi();
      if (Stato_Anta[i] == "O") {
        break;
      }
      delay(10);
    }
    delay(150);
    //Serial.print( "MEMORIZZATO CON SUCCESSO il pos_chiuso su anta "); Serial.println(i);
  }
  // -- FATTO TROVATA POSIZIONE ZERO PER TUTTI

  // -- APRO TUTTO:
  // -- devo iniziare qui il for per fare tutte le ante//
  int i2;

  //Serial.println("INIZIA IL FOR i2");
  for ( i2 = 1; i2 <= (Numero_Ante_Slave ) ; i2 = i2 + 1) {
    inVia(i, "1114",4);
    delay(500);
  }
  // STO APRENDO
  while (true) {
    inVia(i, "0000",5);
    Serial.print("num L'ANTe "); Serial.println(Numero_Ante_Slave);
    for ( i = 1; i <= Numero_Ante_Slave   ; i = i + 1) {
      ricEvi();

      if (Stato_Anta[i] != "F") {
        break;
      }
      if (i == Numero_Ante_Slave) {
        fatto = true;
      }
    }
    delay(100);
    if (button_pressed() == true) {
      ferma_WARD();
      digitalWrite (RELAY1, HIGH);
      return;
    }
    delay(1);
    if (fatto == true) {
      fatto = false;
      break;//chiude while
    }
  }

  
  // -- QUI DEVO MEMORIZZARE IL pos_aperto_slave[i] con le dovute operazioni per la distanza da rispettare
  String posapertotot = "";
  
  for ( int i=1; i<=Numero_Ante_Slave; i = i + 1) {
       pos_aperto[i]=Pos_Anta[i]-100*(Numero_Ante_Slave-i+1);
       posapertotot=posapertotot+pos_aperto[i]+",";
  }
   posapertotot=posapertotot.substring(0,posapertotot.length()-1);
// salva du data.ini
dataSave("pos_aperto",posapertotot);

}// -- FINE set_reset
