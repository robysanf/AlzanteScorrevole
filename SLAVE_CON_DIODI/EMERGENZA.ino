
void emergenza(int rh) {
  //-- QUANDO AVVIENE L'EMERGENZA DOBBIAMO MONITORARE UNA SERIE DI DATI PER POTER CAPIRE COSA HA SCATENATO L'EVENTO
  //-- QUESTI DATI POTREBBERO ESSERE:
  //-- 1 POSIZIONE
  //-- 2 LA TENSIONE
  //-- 3 L'ASSORBIMENTO
  //-- 4 LA DIREZIONE
  //-- 5 LA PROVENIENZA
  //-- POTREMMO INSERIRE QUESTI DATI SUBITO DOPO LA "S" DIVIDENDOLI CON UNA "," E FINENDO LA STINGA CON UN "#".

  Serial.print("\n emergenza = "); Serial.print(rh);

  str_emergenza =  String(v_attuale) + "**" + String(fai_media()) + "**" + String( _Dir) + "**" + String(rh);

  Stato_Anta[0] = 'S';                    // -- questa informazione non verrà mai spedita :-(
  //Ascolta_Master();                       // -- almeno un pacchetto lo mando
  //invia("0000");*********************************************************
  // ** se ricevo un comando lo perdo ammeno che non creiamo una situazione nuova
  // ** che sospende il ciclio di conferma : ad esempio se risposta comando è 9999
  // ** allora sia lo slave che il server ignorano il comando e non cambiano il comando vecchio
  // -- mi salvo la direzione che devo prendere per liberare l'ostacolo prima di fermare altrmenti poi è zero
  if (v_attuale < 0) {
    v_attuale = -v_attuale;
  }
  Serial.print("\n vattuale = "); Serial.print(v_attuale);
  for ( int i = v_attuale; i >= 0; i = i - 15) {
    if (i <= 0) {
      i = 0;
    }
    md.setM2Speed(i * _Dir * motore);
    // @-- Serial.print("\n i = "); Serial.print(i);
    fai_media();
    delay(1);
    if((i%10)==0){
    Ascolta_Master();
    if(str==7777){
        str = 0; str_emergenza = "";
         Stato_Anta[0] = 'P'; 
      }
    }
  }                          
  md.setM2Speed(0);          // -- fermo in rapidità
  digitalWrite(4, LOW);      // DISABILITA I MOTORI
  velocita_crocera = 0;
  v_attuale = 0;
  tensione = 1;
  iCrocera = 0;
  _Dir = _Dir * -1;
  int pos_E = pos + (_Dir * 100 * imp);   // -- calcolo la posizione da raggiungere
  if (pos_E < 0) {                        // -- limito il pos_E a 0 poco +
    pos_E = 0 + quasiChiuso;
  }
  if (pos_E > pos_aperto) {               // -- limito il pos_E a pos_aperto
    pos_E = pos_aperto;
  }
  digitalWrite(4, HIGH);                  // -- abilito il motore
  // @-- velocita_crocera = velocita_crocera_COL;
  // @-- cambiaVelocita(velocita_crocera, 1);    // -- avvio l'anta
  
  
  for ( int i = 0; i <= 600; i++) {        // -- avvio l'anta
    v_attuale = (i * _Dir * motore);
    md.setM2Speed(v_attuale);
    fai_media();
    delay(1);
    if((i%10)==0){
    Ascolta_Master();
    if(str==7777){
        str = 0; str_emergenza = "";
         Stato_Anta[0] = 'P'; 
      }
    }
  } 
                           
  int i = 0;
  int pos_vecchio = 0;
  
  Serial.print("\n pos_E = "); Serial.print(pos_E);
  Serial.print("\n pos = "); Serial.print(pos);
  while ( i < 60) {                     // -- tempo massimo 3 secondi
    if (_Dir == 1 && pos >= pos_E) {      // -- in base alla direzione se vado oltre il pos_E esco
      break;
    }
    if (_Dir == -1 && pos <= pos_E) {     // -- in base alla direzione se vado oltre il pos_E esco
      break;
    }
    if (abs(pos - pos_vecchio ) <= 1 ) {
      // -- QUESTA POTREBBE ESSERE LA PESEUDO VELOCITA' PER CAPIRE CHE SE A 500(lo possiamo anche alzare)
      // -- NON CAMBIA IL POS ALLORA SIAMO BLOCCATI CONTRO IL FINECORSA
      Serial.println("pos invariato reset chiudo stop");
     break;
    }
    pos_vecchio = pos;
    delay(50);                             // -- ritardo
    i++;
  }
  //77777777777777777777777777777777777777777
  for ( int i = v_attuale; i >= 0; i = i - 15) {
    if (i <= 0) {
      i = 0;
    }
    md.setM2Speed(i * _Dir * motore);
    // @-- Serial.print("\n i = "); Serial.print(i);
    fai_media();
    delay(1);
  }                          
  md.setM2Speed(0);          // -- fermo in rapidità
  digitalWrite(4, LOW);      // DISABILITA I MOTORI
  velocita_crocera = 0;
  v_attuale = 0;
  tensione = 1;
  iCrocera = 0;                          // -- fermo
  pos_E = 0;                              // resetto pos_E non si sa mai
 
  _Dir = 0;
}

void set_pos() {
  digitalWrite(4, HIGH);
  md.setM2Speed(-300);
  tensione = 300;
  delay(150);
  md.setM2Speed(0);
  tensione = 1;
  digitalWrite(4, LOW);
  delay(10); // FORSE QUI FUNZIONA MEGLIO
  pos = 0;
  pos_chiuso = pos;
  delay(100); // DEVO VALUTARE SE SERVE
  digitalWrite(4, HIGH);
  Serial.print("TROVATA POSIZIONE ZERO");
  Stato_Anta[0] = 'O';
  delay(1); // DEVO VALUTARE SE SERVE


  // FATTO, TROVATA POSIZIONE 0
}
