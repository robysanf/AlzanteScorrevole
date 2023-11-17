void Partenza_Sicura(int Direzione, int Except, int Prw) {
  //****************************
  //Direzione = 1 APRI
  //Direzione = -1 CHIUDI
  //Execept = 1 uomo assente
  //Execept = 0 uomo presente
  //Prw = VOLORE PER UN EVENTUALE RITARDO
  //****************************
  if (apertura) {
    for (int i = 2 ; i <= Numero_Ante_Slave ; i++) {
      inVia(i, "2223", 44);
      attendi_risposta(i, 5, "2223");
    }
    _Dir = 1;
    String da_scrivere = "Apri" ;
    da_scrivere = da_scrivere + " ** " + String(millis() / 60000);
    writelog(da_scrivere);
    return;
  }
  //---------------------------------------------
  if (_Dir == 0) { // -- (siamo_fermi() == true)
    int i ;
    //**************  APRI
    if ( Direzione == 1) {
      if (dipendente) {
        if (!check_dipendente(200))
        {
          return;
        }
      }
      int curr = 0;
      boolean tutto_alto = check_alzata(true);
      if (!tutto_alto)
      {
        if (Stato_Anta[1] != "T") {
          uno_giu = true;
        }
        return;
      }
      if (Stato_Anta[1] == "A") {      // -- ESCO SENZA FARE NIENTE SE TUTTO ALPERTO
        Serial.println("TUTTO APERTO");
        Serial.println("pos_aperto = " + String(pos_aperto[1]) + " fineriga ");
        return;
      }
      if (Stato_Anta[1] == "T") {
        for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
          inVia(i, "2224", 43);
          attendi_risposta(i, 5, "2224");
        }
      } else {
        //qui viene gestita la partenza quando non è tutto aperto o tutto chiuso
        int i2 = 1;
        boolean collisione = false;
        for (i = 1; i < Numero_Ante_Slave  ; i = i + 1) {
          collisione_[i2] = collisione_anta_[i] - Pos_Anta[i] + Pos_Anta[i + 1];
          if ( collisione_[i2] <= Spazio_Partenza) {
            Anta_Coll[i] = i2;
            i2 = (i2 + 1);
            collisione = true;
          } else {
            Anta_Coll[i] = 0;
          }
        }
        Nr_Coll = i2 ;//Nr_Coll indica IL numero dell'anta che ha fatto collisione
        for ( i = 1 ; i <= Numero_Ante_Slave ; i =  i + 1) {
          // @-- Serial.print("\n Anta_Coll["); Serial.print(i ); Serial.print("] = "); Serial.print(Anta_Coll[i]);
        }
        if (collisione) {// se le ante sono troppo vicine devo far partire prima quella che ha fatto collisione
          Serial.println(" Nr_Coll[");
          int i3;
          int i4;//numero anta da muovere prima
          String temporaneo2 = "";
          // char _temporaneo[32] = "";
          for (i3 = Nr_Coll ; i3 > 0 ; i3 = i3 - 1) {
            if (i3 == Nr_Coll ) {//se ha fatto collisone è la prima che deve partire
              i4 = 0;
            } else {
              i4 = i3;
            }
            int i2;
            for (i2 = 1 ; i2 <= Numero_Ante_Slave ; i2 = i2 + 1) {
              int ritardo_coll = 0;
              if (Anta_Coll[i2] == i4) {//se è l'anta con la collisione
                if ( Anta_Coll[i2] > 0) {
                  ritardo_coll = (ritardo_collisione * (Nr_Coll - 1)) / Anta_Coll[i2];
                }
                inVia(i2, "2224", 41);
                attendi_risposta(i2, 5, "2224");
                delay(200);           // -- RITARDO IN APERTURA SE C'E' IL PERICOLO DI COLLISIONE
              }
            }
            _Dir = 1;
          }
        } else {
          Serial.print("\n arrivato qui");
          for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
            int c = 0;
            inVia(i, "2224", 40); delay(500);
            attendi_risposta(i, 5, "2224");
          }
          _Dir = 1;
        }
      }
      String da_scrivere = "Apri" ;
      da_scrivere = da_scrivere + " ** " + String(millis() / 60000);
      writelog(da_scrivere);
    }
    //**********************************  CHIUDI ******************************
    if ( Direzione == -1) {
      Serial.print("\n partenza");
      if (Stato_Anta[1] == "T" && Stato_Alzata[1] == "D") {
        Serial.print("\n retttll");
        return;
      }
      if ((Pos_Anta[1] < (100 * imp) && digitalRead(CONSENSO_in_lettura) == HIGH)  ) {
        if (check_dipendente(3000))
        { Serial.println("dipppp");
          return;
        }
      }
      if (Stato_Anta[1] == "A") {
        for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
          inVia(i, "2222", 39);
          attendi_risposta(i, 5, "2222");
        }
        _Dir = -1;
      } else {
        int i2 = 1;
        // se lo spazio tra le due ante è sotto la soglia Spazio_Partenza lo metto nel array Anta_Coll[]
        // altrimento Anta_Coll[i] è Zero
        for (i = 1; i < Numero_Ante_Slave  ; i = i + 1) {
          collisione_[i2] = (Pos_Anta[i] - Pos_Anta[i + 1]);
          // @-- Serial.print("\n ******************* collisione_["); Serial.print(i2); Serial.print("] = "); Serial.print(collisione_[i2]);
          if ( collisione_[i2] <= Spazio_Partenza) { //-- VEDREMO COME VA   *coef[i-1]
            Anta_Coll[i] = i2;
            i2 = (i2 + 1);
          } else {
            Anta_Coll[i] = 0;
          }
        }
        Nr_Coll = i2;
        for ( i = 1 ; i <= Numero_Ante_Slave ; i =  i + 1) {
          //Serial.print("\n Anta_Coll["); Serial.print(i); Serial.print("] = "); Serial.print(Anta_Coll[i]);
        }
        if ( Nr_Coll != 0) {
          Serial.print("\n ncoll");
          int i3; //
          int i4;
          String temporaneo2 = "";
          // char _temporaneo[32] = "";
          for (i3 = Nr_Coll ; i3 > 0 ; i3 = i3 - 1) {
            Serial.print("\n ncoll333333");
            if (i3 == Nr_Coll ) {
              i4 = 0;
            } else {
              i4 = i3;
            }
            int i2;
            for (i2 = 1 ; i2 <= Numero_Ante_Slave   ; i2 = i2 + 1) {
              int ritardo_coll = 0;
              if (Anta_Coll[i2] == i4) {
                inVia(i2, "2222", 39);
                attendi_risposta(i2, 5, "2222");
                delay(500);           // -- RITARDO IN APERTURA SE C'E' IL PERICOLO DI COLLISIONE
              }
            }
            _Dir = -1;
          }
        } else {
          for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
            inVia(i, "2222", 38);
            attendi_risposta(i, 5, "2222");
          }

          _Dir = -1;

          Serial.println(" partenza senza collisione");
        }
      }
      String da_scrivere = "Chiudi" ;
      da_scrivere = da_scrivere + " ** " + String(millis() / 60000);
      writelog(da_scrivere);
    }
  } else {
    Serial.println("fermo perchè gia in movimento");
    ferma_WARD();
  }
}

boolean check_dipendente(int contatore) {

  Serial.println("DENTRO check_dipendente 1");
  Serial.println(" CONSENSO_in_lettura AA= " + String(digitalRead(CONSENSO_in_lettura)));
  for (int i = 1; i < contatore; i = i + 1) {
    Serial.println(" CONSENSO_in_lettura AA= " + String(digitalRead(CONSENSO_in_lettura)));
    delay(20);
    if (digitalRead(CONSENSO_in_lettura) == LOW) {
      Serial.println(" CONSENSO_in_lettura = " + String(digitalRead(CONSENSO_in_lettura)));
      return true;
    }



  }
  //Serial.println(" CONSENSO_in_lettura = " + String(digitalRead(CONSENSO_in_lettura)));
  return false;
}


boolean check_alzata(boolean invia) {
  boolean  tutto_alto = true;
  for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
    if (Stato_Alzata[i] != "U") {
      if (invia) {
        if (i == 1) {
          if (Stato_Anta[1] == "T" && U_P == true) {
            inVia(i, "22241" + mm_apri_fisso, 36);
            attendi_risposta(i, 5, "2224");
            apertura = true;
          } else
          {
            inVia(i, "22242", 35);
            attendi_risposta(i, 5, "2224");
          }
        }
        else {
          //Serial.print("\n apri denrtro check alzata");
          inVia(i, "22242", 34);
          attendi_risposta(i, 5, "2224");
        }
      }
      tutto_alto = false;
    }
  }
  return tutto_alto;
}




boolean evento_pulsanti() { // -- NON USATO 08/12/2022
  if (digitalRead(APRI) == LOW || digitalRead(CHIUDI) == LOW ||  digitalRead(STOP) == LOW)
  {
    return true;
  }
  return false;
}
