void check_pos()  {

  //***************************************************
  /*if (pos_apri_fisso!=0 && pos>=pos_apri_fisso ) {
    Serial.print("STOP >> ");
    ferma_WARD(7);
  } */
  //*****************************************************
    //----------------------------------------------------
if (pos_apri_fisso!=0 && pos>=pos_apri_fisso ) {
    if(si_stop_sblocco){
     Serial.print("STOP >> apri fisso");
     si_stop_sblocco=false;
     pos_apri_fisso = 0;
     ferma_WARD(7);
     return;                                   // -- NON TESTATO    
    }
     velocita_crocera = velocita_crocera_MAX;  // -- NON TESTATO            
     cambiaVelocita(velocita_crocera,1);       // -- NON TESTATO
     pos_apri_fisso = 0;  
  } 

    //--------------------------------------------------
  if (pos <= (quasiChiuso + pos_chiuso) && _Dir == -1 ) {
    porta_tutta_chiusa = true;
    porta_tutta_aperta = false;
    Serial.print("STOP >> ");
    ferma_WARD(7);
    Abbassa_Anta();

    Stato_Anta[0] = 'T';
  }                               //-- ferma quando totto chiuso --
  if (pos <= (quasiChiuso + pos_chiuso) && _Dir == 0 && Stato_Anta[0] != 'S' && Stato_Anta[0] != 'P') {
    if (set == 1) {
      Stato_Anta[0] = 'T';
    }
    porta_tutta_chiusa = true;
    porta_tutta_aperta = false;
  }

  if (pos >= pos_aperto && _Dir == 1 ) {
    porta_tutta_chiusa = false;
    porta_tutta_aperta = true;
    ferma_WARD(7);
    Stato_Anta[0] = 'A';
  }                             //-- ferma quando tutto aperto --
  if (pos >= pos_aperto && _Dir == 0 ) {
    Stato_Anta[0] = 'A';
    porta_tutta_chiusa = false;
    porta_tutta_aperta = true;
  }
  if (pos <= pos_aperto && pos >= (quasiChiuso * 2.5 + pos_chiuso) && _Dir == 0 && Stato_Anta[0] != 'S') {
    Stato_Anta[0] = 'F';
    porta_tutta_chiusa = false;
    porta_tutta_aperta = false;
  }
  int verso = 1;
  if (Stato_Anta[0] == 'V' || Stato_Anta[0] == 'C' ) {

    //-- devo rallentare in finecorsa aperto e chiuso--
    if ((_Dir == 1 && pos >= (pos_aperto - spazioRallenta)) || ( _Dir == -1 && pos <= (pos_chiuso + spazioRallenta)))
    { Serial.println("sono in spazio rallenta  ");
      int i = 0;
      int delay_finecorsa = 3;
      // a che

      velocita_crocera = velocita_crocera_MIN;            // stabilisco il target
      cambiaVelocita(velocita_crocera,1);

      Stato_Anta[0] = 'L';
    }
  }

  // ---------------------------------------
  if (Stato_Anta[0] == 'L' && _Dir == -1 ) {
    //-- devo rallentare in finecorsa CHIUSO  --
    if (( pos <= (pos_chiuso + spazioRallenta) / 5) && velocita_crocera != velocita_crocera_CHI)
    { Serial.println("sono in spazio rallenta CHIUSO  ");
      velocita_crocera = velocita_crocera_CHI;               // stabilisco il target
      cambiaVelocita(velocita_crocera,1);

      Stato_Anta[0] = 'L';
    }
  }

} //FINE --- check_pos ---

boolean spazio_fine_corsa() {
  if ((_Dir == 1 && pos >= (pos_aperto)) || ( _Dir == -1 && pos <= (pos_chiuso + quasiChiuso)))
  { //Serial.println("wwwwspazio_fine_corsa");
    return true;
  }
  return false;

}
