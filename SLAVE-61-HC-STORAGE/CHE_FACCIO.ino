void che_faccio() {                                //Serial.println(" inizio che faccio  ");//Serial.println(Pulsanti);
  /*int packetSize = Udp.parsePacket();
    if (packetSize)
    {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0){ packetBuffer[len] = 0;}
    String mystr=String(packetBuffer);
    Serial.print(mystr);
    str=mystr.substring(0,4).toInt();
    str2=mystr.substring(4);
    Udp.endPacket();
    }*/


  //if (set == 0) {  // ** comandi per il SET RESET ***************************************
  switch (str) {
    case 1111: // -- chiudo in set reset
      Serial.print("\n chiudo in set reset");
      Stato_Anta[0] = 'F';
      set_reset_chiudo(); str = 0; Stato_Anta[0] = 'R';
      break;
    case 1112: // -- metto in freno
      Serial.print("\n metto in freno");
      //set_reset_freno();str=0;
      break;
    case 1113: // -- tolgo il freno
      Serial.print("\n tolgo il freno");
      // set_stop_freno();str=0;
      break;
    case 1114: // -- apro in set reset
      Serial.print("\n apro in set reset");
      set_reset_apro(); str = 0;
      break;
    case 1115: // -- memorizzo la posizione 0 in set reset
      Serial.print("\n apro in set reset");
      set_reset_pos(); str = 0;
      break;
    case 1116: // -- avvia il test dei limit e poi cominicia ad inviare la str_limit
      Serial.print("\n richiesta valori limit");
      set = 1 ;
      str = 0;
      Stato_Anta[0] = 'F';
      break;
    case 1117: // -- stop invio limit
      Serial.print("\n stop richiesta valori limit");
      str_limit = "";
      str = 0;
      break;
    case 1118: // -- stop invio limit
      Serial.print("\n Set limit");
      setLimitCurve();
      str = 0;
      break;
      case 1119: 
      Serial.print("\n alzata");
      Stato_Anta[0] = 'F';
      Alza_Anta();
      break;
      case 1120: 
      Serial.println( " inizio controllo assorbimento continuo ");
      controlla_assorbimento();
      stampa_assorbimento();
      break;
  }
  // }
String except="";
String mm="";
  switch (str) { // ** comandi per LE FUNZIONI APRI, CHIUDI, etc... **********************
    case 2221: // -- FERMA
      digitalWrite(led, LOW);
      Serial.print("\n STOP");
      bloc = true; 
      str = 0;  
      bloc = false;
      if(( _Dir == -1 && pos <= (pos_chiuso + spazioRallenta) / 4)){
         break;
        }
        ferma_WARD(7);
      break;
    case 2222: // -- CHIUDI
      Serial.print("\n CHIUDI");
      if (str2 == "") {
        str2 = "0";
      }
      WARD(-1, 0, str2.toInt(),0); str = 0;
      break;
    case 2223: // -- APRI
      digitalWrite(led, HIGH);
      Serial.print("\n APRI");
      if (str2 == "") {
        str2 = "0";
      }
      WARD(1, 0, str2.toInt(),0); str = 0;
      break;
    case 2224: // -- APRI CON SBLOCCO
      digitalWrite(led, HIGH);
      Serial.print("\n APRI CON SBLOCCO");
      if (str2 == "") {
        str2 = "0";
      }
      except = str2.substring(0,1);
      mm = str2.substring(1);
      WARD(1, except.toInt(), 0, mm.toInt()); 
      except="";
      mm="";
      str = 0;
      break;
    case 2225: // -- FAST PER NON ANDARE IN COLLISIONE
      Serial.print("\n APRI CON SBLOCCO");
      fast_ward(); str = 0;
      break;
    case 2226: // -- PER INIZIARE IL SET RESET NON SERVE NON USATO
      Serial.print("\n IMPOSTO A ZERO IL POS");
      set_pos(); str = 0;
      break;
    case 7777: // -- PER INIZIARE IL SET RESET NON SERVE NON USATO
      Serial.print("\n RESET EMERGENZA");
      str = 0; str_emergenza = "";
      Stato_Anta[0] = 'F'; 
      break;
    case 5555: 
      Serial.print("\n cambia canale");
      cambia_canale(str2);
      break;

    // ************ comandi per IMPOSTARE LE VARIABILI ***************************************

    /*    case 3330: // -- APRO PER VERIFICARE IL NOVIMENTO
            Serial.print("\n apro in set reset");
            set_reset_apro();
            break;
    */
    case 3331: // -- PER CAMBIARE LA VELOCITA' MASSIMA
      str3 = str2.toInt() ;
      velocita_crocera_MAX = str3 / 100.00;
      // spazioRallenta = spazioRallenta_hold*(1+(velocita_crocera_MAX/50)); // mettere una variabile assoluta di riferimento
      Serial.print("\n velocita_crocera_MAX = "); Serial.print(velocita_crocera_MAX);
      Serial.print("\n spazioRallenta = "); Serial.print(spazioRallenta / imp);
      break;
    case 3332: // -- PER CAMBIARE LA VELOCITA' MINIMA
      str3 = str2.toInt() ;
      velocita_crocera_MIN = str3 / 100.00;
      Serial.print("\n velocita_crocera_MIN = "); Serial.print(velocita_crocera_MIN);
      break;
    case 3333: // -- PER CAMBIARE LA VELOCITA' IN COLLISIONE  OPEEN CLOSE FAAST SETT1
      str3 = str2.toInt() ;
      velocita_crocera_COL = str3 / 100.00;
      Serial.print("\n velocita_crocera_COL = "); Serial.print(velocita_crocera_COL);
      break;
    case 3334: // -- PER CAMBIARE LA DIREZIONE DEL MOTORE
      motore = str2.toInt();
      Serial.print("\n motore = "); Serial.print(motore);
      break;
    case 3335: // -- PER CAMBIARE LA DIREZIONE DELL'ENCODER
      encoder = str2.toInt();
      Serial.print("\n encoder = "); Serial.print(encoder);
      break;
    // -- OPPURE
    case 3336: // -- PER CAMBIARE IL VALORE DI ATTRITO
      attrito = str2.toFloat();
      Serial.print("\n attrito = "); Serial.print(attrito);
      break;
    case 3337: // -- E' LIBERO
      //ritardo=str2.toFloat();
      // Serial.print("\n ritardo = ");Serial.print(ritardo);
      break;
    case 3338: // -- PER CAMBIARE IL VALORE DI INERZIA
      inerzia = str2.toInt();
      Serial.print("\n inerzia = "); Serial.print(inerzia);
      break;
    case 3339: // -- PER CAMBIARE IL VALORE DI PESO
      peso = str2.toInt();
      marginelimite=peso;
      Serial.print("\n peso = "); Serial.print(peso);
      break;
    case 3310: // -- PER SETTARE IL VALORE DI POS_APERTO
      pos_aperto = str2.toInt();
      Serial.print("\n pos_aperto = "); Serial.print(pos_aperto);
      break;
    case 3311: // -- PER SETTARE IL VALORE DI POS_APERTO
      str3 = str2.toFloat() ;
      imp = str3 / 100.00;
      Serial.print("\n imp = "); Serial.print(imp);
      imp_vel = ((1 / (imp / (cadenza * 2))) * 1000000);
      spazioRallenta = spazioRallenta_mm * imp;  
      quasiChiuso = quasiChiuso_mm * imp; 
      spazio_no_chk_vel = spazio_no_chk_vel_mm * imp;
  
      break;
   case 3312: // -- PER CAMBIARE LA VELOCITA' MINIMA
      str3 = str2.toInt() ;
      velocita_crocera_SET = str3 / 100.00;
      Serial.print("\n velocita_crocera_MIN = "); Serial.print(velocita_crocera_MIN);
      break;
    case 3340: // -- PER SETTARE I LIMITI DELLA CURVA
      spacchetta_limit(str2);
      Serial.print("\n carica limit = "); Serial.print(str2);
      break;
  }
  str = 0; str2 = ""; str3 = 0.00;
}
void cambia_canale(String ch){
     digitalWrite(11, LOW);
     delay(150);
      String s="AT+C"+ch;
      char lis[255];
      s.toCharArray(lis,255);
     Serial1.write(lis);
     delay(150);
     digitalWrite(11, HIGH);
  }
