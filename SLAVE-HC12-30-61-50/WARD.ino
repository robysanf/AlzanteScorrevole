//*******************************************************
// WARD
//*******************************************************
void WARD(int dir, int except, int prw, int mm) {
  if (_Dir != 0) { // -- FERMO SE IN MOVIMENTO
    ferma_WARD(7);
    return;
  }
  if ( dir == -1 && porta_tutta_chiusa == true) {
    Abbassa_Anta();
    return;
  }

  //******* unico punto in cui si definisce _DIR != 0 utilizzato da altre parti nel codice
  _Dir = dir;

  //**************
  // in base a dir e pos viene decisa la velocita a cui arrivare
  //**************

  velocita_crocera = check_posizione_WARD(dir);
  float delta = velocita_crocera;
  Serial.print("\n velocita_crocera === " ); Serial.println(velocita_crocera);
  Serial.print("\n Stato_Alzata[0] === " ); Serial.println(Stato_Alzata[0]);
  //SE IL FINE CORSA E' CORTOCIRCUITATO QUINDI L'ANTA è ARRIVATA IN BASSO
  //OPPURE IN OGNI CASO LO STATO_ALZATA NON E' ALTO
  if ( digitalRead(A7) == LOW || Stato_Alzata[0] != 'U') { //-- *****ATTENZIONE**** METTERE VARIABILE CON A7

    Alza_Anta();
    Serial.print("\n ultimo comando === " ); Serial.println(str_old);
    Serial.print("\n ultimo comando === " ); Serial.println(str);
    Ascolta_Master();
    Serial.print("\n ultimo comando === " ); Serial.println(str_old);
    Serial.print("\n ultimo comando === " ); Serial.println(str);
    //******* quando chiamo ward con except=1 fa solo ABBASSA_ANTA o ALZA_ANTA
    // verificare versione indonesia ASHIOK come gestisce apertura
    //***********************************************************
    if (except == 1) {
      pos_apri_fisso = mm * imp;
      velocita_crocera = velocita_crocera_COL; // -- MEGLIO METTERE velocita_crocera_COL OPPURE ACCELERARE DOPO I mm A velocita_crocera_MAX IN check_pos()
      delta = velocita_crocera;

    }
    if (except == 2) {
      ferma_WARD(7);
      str = 0;
      return;

    }
  }
  if (str == 2221 && mm == 0) {
    Serial.print("\n ricevuto 2221 in wARD === " );
    ferma_WARD(7);
    str = 0;
    return;
  }
  porta_tutta_chiusa = false;
  v_attuale = 0;

  // ritardo con pwr>0 e aspetto altri comandi
  for ( int k = 0 ; k < prw && Stato_Anta[0] != 'L' ; k++ ) {
    if (String(azione).substring(0, 4) == "2221" ) {
      ferma_WARD(7);
      azione[0] = '\0';
      return;
    }
    if (String(azione).substring(0, 4) == "2225" ) {
      fast_ward();
      azione[0] = '\0';
      return;
    }
    delay(1);
  }
  digitalWrite(4, HIGH);
  delay(1);
  int  i ;
  int rob = 3;// ibrido 0 astec 2
  float velocita = 0.00;
  V_M=0.00;
  //******** accelero fino a topSpeed
  for ( i = 90; i <= TopSpeed; i = i + 1 + rob) {
    velocita = Tachi_Metro();
    fai_media();
    md.setM2Speed(i * dir * motore);
    tensione = (i * dir * motore) + 1;
    v_attuale = (i * dir * motore);
    Serial.print("\n  wARD i === " ); Serial.print(i);
    if (limit()) {
      v_attuale = i;
      emergenza(3);
      return;
    }
    if (crocera(1)) {
      iCrocera = (i);
      Serial.print("ok crocera ward " + String(i));
      break;
    }
    if ( dir == -1)
    {
      if (velocita > delta * 0.5 && velocita < delta * 0.7) {
        delay(3 * inerzia);
      }
      if (velocita > delta * 0.7) {
        delay(10 * inerzia);
      }
      if (pos <= (pos_chiuso + quasiChiuso)) {
        v_attuale = i;
        porta_tutta_chiusa = true;
        ferma_WARD(7);
        Abbassa_Anta();
        return;
      }
      //PERCHè? questo non verrà mai eseguito
      if ( porta_tutta_chiusa == true) {
        v_attuale = i;
        ferma_WARD(7);
        return;
      }
    }
    if ( dir == 1)
      // ----------------------------
    {
      if (velocita > delta * 0.5 && velocita < delta * 0.7) {
        delay(3 * inerzia);
      }
      if (velocita > delta * 0.7) {
        delay(10 * inerzia);
      }
      if ( pos >= pos_aperto) {
        porta_tutta_aperta = true;
        v_attuale = i;
        ferma_WARD(7);
        return;
      }
      //PERCHè?questo non verrà mai eseguito
      if ( porta_tutta_aperta == true) {
        v_attuale = i;
        ferma_WARD(7);
        return;
      }
    }
    Ascolta_Master();
    if (str == 2221) {
      Serial.print("\n ricevuto 2221 in wARD for === " );
      if (mm == 0) {
        ferma_WARD(7);
        str = 0;
        return;
      } else
      { Serial.print("\n ricevuto 2221 con sblocco" );
        si_stop_sblocco = true;
      }
    }
    //------------------------------------------------
    //delay(5);
  }//FINE for

  if (dir == 1) {
    porta_tutta_chiusa = false;
  }
  if (dir == -1) {
    porta_tutta_aperta = false;
  }
  v_attuale = (i * dir * motore);// in piu
  tensione = (i * dir * motore);
}

//*********************************************
// FERMA_WARD
//*********************************************
void ferma_WARD( int passo) {     // DEVO FERMARE ho introdotto il passo così posso chiamare un ferma lento o un ferma veloce
  Serial.print("\n ferma ward");
  Serial.print("\n fermaward v_attuale = " + String(v_attuale));
  Stato_Anta[0] = 'F';
  pos_apri_fisso = 0;
  si_stop_sblocco = false;
  if (v_attuale < 0) {
    v_attuale = -v_attuale; // positivizzo la v_attuale
  }
  int instant_pos;
  int i;
  int fatto = 0;
  int rob = 3 ;//0 ibrido 2 astec
  for ( i = v_attuale; i >= 0; i = i - (passo + rob) ) {
    fai_media();
    instant_pos = pos;
    //PERCHè? per poter spingere mentre abbassa?
    if (pos <= 3) {
      Serial.print("STOP 2 >> ");
      Serial.println(instant_pos);
      break;
    }
    if (i <= 0) {
      i = 0;
    }
    if (Tachi_Metro() < velocita_crocera * 0.8 && fatto < 1) {
      passo = passo + 10;
      fatto = 1;
    }
    if (Tachi_Metro() < velocita_crocera * 0.5 && fatto < 2) {
      passo = passo + 15;
      fatto = 2;
    }
    tensione = i + 1;//il piu 1 per fare in modo che non sia zero??
    md.setM2Speed(i * _Dir * motore);
    delay(10);
    /* if (limit()) {
       emergenza(4);
       break;
      }*/
  }
  md.setM2Speed(0);
  for (int ic = 0; ic < 20; ic++) {
    //Serial.print("corrente " + String(ic) ); Serial.println(" = " + String(corrente[ic]));
    corrente[ic] = 0;
  }
  for (int ic = 0; ic < 5; ic++) {
    Serial.print("veli " + String(ic) ); Serial.println(" = " + String(test_vel[ic]));
    test_vel[ic] = 0;
  }
  velocita_misurata = 0;
  velocita_crocera = 0;
  v_attuale = 0;
  tensione = 1;
  iCrocera = 0;
  _Dir = 0;         // ANCHE IN EMERGENZA METTE _DIR=0
  apri = false;
  chiudi = false;
  Stato_Anta[0] = 'F';
  if (porta_tutta_chiusa == true ) {
    Stato_Anta[0] = 'T';
  }
  if (porta_tutta_aperta == true ) {
    Stato_Anta[0] = 'A';
  }
  if (set == 1) {
  }
  str_emergenza = "";
  delay(50);
  digitalWrite(4, LOW);   // DISABILITA I MOTORI
  Serial.print("FINE FERMA WARD ");
}

//******************************************
// CONTROLLA_VELOCITA_WARD
//*****************************************
void controlla_velocita_WARD() {
  fai_media();
  velocita_misurata = Tachi_Metro();
  if ((micros() - Tempo_Reazione_Emergenza) > 200000) {
    Tempo_Reazione_Emergenza = 0;
    Conto_Emergenze = 0;
  }
  velocita_misurata = Tachi_Metro();
  if (velocita_misurata < (velocita_crocera * (attrito + (velocita_crocera  * 0.02 * 0.01))) && pos > 12 * imp) {
    fai_media();
    //Serial.print("Velocita misurata : "); Serial.println(velocita_misurata);
    //Serial.print("velocita_crocera : " ); Serial.println(velocita_crocera);
    //Serial.print("icrocera : "); Serial.println(iCrocera);
    //Serial.print("_Dir : "); Serial.println(_Dir);
    //Serial.print("v_attuale : " ); Serial.println(v_attuale);
    //Serial.print("attrito calcolato: " ); Serial.println(attrito + (velocita_crocera  * 0.02 * 0.01));
    //Serial.print("vel * atrito : "); Serial.println(velocita_crocera * attrito);
    if (Tempo_Reazione_Emergenza == 0) {
      Tempo_Reazione_Emergenza = micros();
      //Serial.print("TEMPO EMERGENZA : "); Serial.println(Tempo_Reazione_Emergenza);
    }
    Conto_Emergenze = Conto_Emergenze + 1;
    //Serial.print("CONTO EMERGENZA : "); Serial.println(Conto_Emergenze);
    if ((micros() - Tempo_Reazione_Emergenza) > 100000 || Conto_Emergenze > 3) {
      emergenza(16);
      Conto_Emergenze = 0;
      Tempo_Reazione_Emergenza = 0;
      return;
    }
  }
  fai_media();
  if (_Dir != 0) {
    if (velocita_misurata > (velocita_crocera * 1.01)) {
      if (velocita_misurata > (velocita_crocera * 1.02)) {
        if (velocita_misurata > (velocita_crocera * 1.03)) {
          //Serial.print("   ------   ");
          v_attuale = v_attuale - 10 * (_Dir * motore);
          md.setM2Speed(v_attuale);
          //Serial.println(v_attuale);
          tensione = v_attuale;
          return;
        }
        //Serial.print("   ----   ");
        v_attuale = v_attuale - 8 * (_Dir * motore);
        md.setM2Speed(v_attuale);
        //Serial.println(v_attuale);
        tensione = v_attuale;
        return;
      }
      //Serial.print("   --   ");
      v_attuale = v_attuale - 5 * (_Dir * motore);
      md.setM2Speed(v_attuale);
      //Serial.println(v_attuale);
      tensione = v_attuale;
      return;
    }


    if ( velocita_misurata < (velocita_crocera * 0.99) && ( abs(v_attuale) ) < ((1500 - iCrocera) * 0.05 + ( iCrocera))  ) { //&& zona_no_control == false
      //Serial.print("Calcolo : "); Serial.println((500 - iCrocera) * 0.05 + ( iCrocera));

      if (velocita_misurata < (velocita_crocera * 0.98)) {
        if (velocita_misurata < (velocita_crocera * 0.97)) {
          //Serial.print("   ++++++  ");
          v_attuale = v_attuale + 15 * (_Dir * motore);
          md.setM2Speed(v_attuale);
          //Serial.println(v_attuale);
          tensione = v_attuale;
          return;
        }
        //Serial.print("   ++++  ");
        v_attuale = v_attuale + 10 * (_Dir * motore);
        md.setM2Speed(v_attuale);
        //Serial.println(v_attuale);
        tensione = v_attuale;
        return;
      }
      //Serial.print("   ++  ");
      v_attuale = v_attuale + 5 * (_Dir * motore);
      md.setM2Speed(v_attuale);
      //Serial.println(v_attuale);
      tensione = v_attuale;
      return;
    }
  }
} // FINE controlla_velocita

//*****************************************************
// CHECK_POSIZIONE_WARD
//*****************************************************
float check_posizione_WARD(int _dir) { // determino qui il targhet da raggiungere in base alla posizione e la direzione (solo in partenza)
  //Serial.print(" _dir = "); Serial.println(_dir);
  //Serial.print(" set = "); Serial.println(set);
  if ( _dir == 0 ) {
    return 0; // -- nel caso 0 significasse ferma
  }
  if (( pos >= (pos_aperto - spazioRallenta)) && set == 1 && _dir == 1 ) {
    Stato_Anta[0] = 'L';
    Serial.println(" targhet = velocita_crocera_MIN");
    return velocita_crocera_MIN;
  }
  if (( pos <= (pos_chiuso + spazioRallenta)) && set == 1 && _dir == -1 ) {
    Stato_Anta[0] = 'L';
    Serial.println(" targhet = velocita_crocera_MIN");
    return velocita_crocera_MIN;
  }
  if ( pos >= (pos_chiuso + spazioRallenta) || pos <= (pos_aperto - spazioRallenta) && set == 1 ) {
    Stato_Anta[0] = 'V';
    Serial.println(" targhet = velocita_crocera_Max");
    return velocita_crocera_MAX;
  }
}
//**********************************************
//   FASTWARD
//**********************************************
void fast_ward() {
  Serial.print("\n FAST ward");
  //Serial.print("\n Stato_Anta" + String(Stato_Anta));
  if ( Stato_Anta[0] == 'V' && _Dir != 0 && Stato_Anta[0] != 'L') {
    int i;
    int Step;

    //  DEVO FARE UN CONFRONTO TRA VEL MAX E VEL COLL PER DECIDERE LO STEP DI ACCELERAZIONE
    //  STEP = VEL_MAX-VEL COLL 15-6 / 2 LO TRASFORMO IN INT E MI VIENE UN NUMERO SENZA VIRGOLE

    velocita_crocera = (velocita_crocera_COL);
    //Serial.print("\n Tachi_Metro =" + String(Tachi_Metro()));
    cambiaVelocita(velocita_crocera, 1);
    Stato_Anta[0] = 'C';
    //Serial.print("\n Stato_Anta" + String(Stato_Anta));
  }
}

//*****************************************************************
//    cambia velocità
//    decide quanto rapidamente variare la velocità da un valore ad un'altro
//    la variabile "salto_velocita" va da 0 - 1 float determina il livello a cui intervenire per cambiare la rapidità
//    la variabile "rapidita" va da 0 - 5 riduce la rapidità di variazione quanto piu è alta
//*****************************************************************
void cambiaVelocita(int velocita_crocera_target, int passo) {
  int verso = 1;
  float delta = abs(Tachi_Metro() - velocita_crocera_target);
  //Serial.println("V_M " + String(Tachi_Metro()));
  if (velocita_crocera_target < Tachi_Metro()) {
    verso = -1;
  }
  if (v_attuale < 0) {
    v_attuale = -v_attuale;
  }
  int i = 0;

  boolean test1 = true; boolean test2 = true; boolean test3 = true;
  for ( i = v_attuale; i >= 0; i = i + verso * passo )
  {
    md.setM2Speed(i * _Dir * motore);
    v_attuale = i * _Dir * motore ;
    tensione = i * _Dir * motore;
    fai_media();
    if (verso == 1) {
      if (Tachi_Metro() > velocita_crocera_target - (delta * 0.3)) {
        delay(2 * inerzia);
      }
      if (crocera(verso)) {
        iCrocera = (i);
        //Serial.println("ok crocera verso1 " + String(i));
        break;
      }
      if (spazio_fine_corsa() && Stato_Anta[0] == 'C') {
        return;
      }
    }//end if verso = 1
    if (verso == -1) {
      float v = Tachi_Metro();
      if (Tachi_Metro() > velocita_crocera_target + (delta * 0.7) ) {
        delay(1);
        if (test1) {
          //Serial.print("V_M1 " + String(v)); Serial.println("target1 " + String(velocita_crocera_target + (delta * 0.7) ));
          test1 = false;
        }

      }
      if ((Tachi_Metro() < velocita_crocera_target + (delta * 0.5)) &&  (Tachi_Metro() > velocita_crocera_target + (delta * 0.3))) {
        delay(2 * inerzia);
        if (test2) {
          //Serial.print("V_M2 " + String(v)); Serial.println("target2 " + String(velocita_crocera_target + (delta * 0.5) ));
          test2 = false;
        }

      }
      if (Tachi_Metro() < velocita_crocera_target + (delta * 0.3) ) {
        delay(4 * inerzia);
        if (test3) {
          //Serial.print("V_M3 " + String(v)); Serial.println("target3 " + String(velocita_crocera_target + (delta * 0.3) ));
          test3 = false;
        }
      }
      if (crocera(verso)) {
        iCrocera = (i);
        Serial.println("ok crocera verso -1 " + String(i));
        break;
      }
      if (spazio_fine_corsa() && Stato_Anta[0] == 'C') {///***************************************testare serve C
        return;
      }
    }//end if verso -1
    if (spazio_fine_corsa()) {
      if (_Dir == -1) {
        ferma_WARD(7);
        Abbassa_Anta();
        return;
      }
      if (_Dir == 1) {
        ferma_WARD(7);
        return;
      }
    }
    ///****************VERIFICARE QUANTO PESA**************************************
    Ascolta_Master();
    // Serial.println("stato ");Serial.print(Stato_Anta[0]);
    if (str == 2221 && Stato_Anta[0] != 'S' ) {
      //Serial.println("2221 denrtro cambio vel");
      ferma_WARD(7);
      str = 0;
      return;
    }
    if (str == 7777 ) {
      str_emergenza = "";
    }
    //**********************************************
    tensione = i * _Dir * motore;
    if (limit()) {
      v_attuale = i * _Dir * motore ;
      tensione = i * _Dir * motore;
      emergenza(6);
      return;
    }
  }//fine for
  v_attuale = i * _Dir * motore ;
  tensione = i * _Dir * motore; //ha senso usare tensione solo per limit?
}
