void Check_Pin(){                             // -- COSTRUISCO LA STRINGA DA SWITCIARE
  //int i;
  int Pulsanti = 0;
  if (digitalRead(APRI) == LOW){Pulsanti = 2000;} else {Pulsanti = 1000;} 
  if (digitalRead(CHIUDI) == LOW){Pulsanti += 20;} else {Pulsanti += 10;}
  if (stato_APRI == 1){ Pulsanti += 100;} 
  if (stato_CHIUDI == 1){Pulsanti += 1;} 
  if (Pulsanti == _Pulsanti){return;}
  _Pulsanti = Pulsanti;
   Serial.print("\n dentro Pulsanti = ");Serial.println(Pulsanti);
if (U_P == true && set == 1){
    switch (Pulsanti) {
      
      case 2010: // -- APPENA PREMUTO APRI
       Serial.println("dentro apri");
        if (_Dir != 0){                                        // -- SE SONO IN MOVIMENTO > FERMO                             
        ferma_WARD();
        Serial.println("dentro STOPoo");
        return;
        }                                                                          
        delay(50); if(digitalRead(APRI)!=LOW){ return;}        // -- ALTRIMENTI ASPETTO 200
        T_scemo_2 = millis();                                  // -- > GUARDO IL TEMPO
        if ((T_scemo_2 - T_scemo_1) > 100 ){                   // -- ANTI SCEMO, SE IL TEMPO CHE PASSA DALL'ULTIMA VOLTA CHE HO RILASCIATO UN PULSANTE  E' SUPERIORE A 500..
           Partenza_Sicura(1,0,0);                             // -- > APRE
           Serial.println("dentro apri>>");
           stato_APRI = 1;                                     // -- > IMPOSTA IL PULSANTE SU PREMUTO
        } 
        break;
//------------------------------------------------------------------------------------------------------------       
      case 2110 :                // -- PREMUTO STABILMENTE APRI
        break;                                                // -- NON FACCIO NIENTE
//------------------------------------------------------------------------------------------------------------       
      case 1110 : {             // -- APPENA RILASCIATO APRI
        ferma_WARD();                                         // -- DEVO FERMARE IL MOVIMENTO
        stato_APRI = 0;                                       // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
        T_scemo_1 = millis();}
        apertura = false;
        break; 
//------------------------------------------------------------------------------------------------------------     
      case 1020:  // -- APPENA PREMUTO CHIUDI
        if (_Dir != 0){ ferma_WARD(); return;}                                                         // -- SE SONO IN MOVIMENTO FERMO                                                                         
        delay(50); if(digitalRead(CHIUDI)!=LOW){ return;}                                             // -- ALTRIMENTI ASPETTO 300     Serial.println("sotto tempo 300 P_U1 1020");
        T_scemo_2 = millis();                                                                          // -- > GUARDO IL TEMPO
           if ((T_scemo_2 - T_scemo_1) > 100 ){//Serial.println("chiudo P_U1 1020");                   // -- ANTI SCEMO, SE IL TEMPO CHE PASSA DALL'ULTIMA VOLTA CHE HO RILASCIATO UN PULSANTE  E' SUPERIORE A 500.. 
              Partenza_Sicura(-1,0,0);                                                                 // -- > CHIUDE
              //-@- Serial.println("dentro <<");
              stato_CHIUDI = 1;                                                                        // -- > IMPOSTA IL PULSANTE SU PREMUTO
           }
        break;
//------------------------------------------------------------------------------------------------------------        
      case 1021 :  // -- PREMUTO STABILMENTE CHIUDI
        break;     // -- NON FACCIO NIENTE
//------------------------------------------------------------------------------------------------------------        
      case 1011 :  {// -- APPENA RILASCIATO CHIUDI
          ferma_WARD();                                                   // -- DEVO FERMARE IL MOVIMENTO 
          stato_CHIUDI = 0;                                               // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
          //-@- Serial.println("dentro STOPoooo");
          T_scemo_1 = millis();}
        break;
//------------------------------------------------------------------------------------------------------------ 
      case 2011 :  // -- APPENA FATTO SCAMBIO PULSANTE TRA APRI E CHIUDI
            {ferma_WARD();                                                        // -- DEVO FERMARE IL MOVIMENTO 
            stato_CHIUDI = 0;                                                     // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
            //-@- Serial.println("dentro STOPooooo");
            T_scemo_1 = millis();}
          break;  
  
        case 1120 :  // -- APPENA FATTO SCAMBIO PULSANTE TRA CHIUDI E APRI
            {ferma_WARD();                                                        // -- DEVO FERMARE IL MOVIMENTO 
             stato_APRI = 0;                                                       // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
             //-@-Serial.println("dentro STOPoooooo");
             T_scemo_1 = millis();}
          break;   

        case 1010 :  // -- APPENA FATTO SCAMBIO PULSANTE TRA CHIUDI E APRI
            {ferma_WARD();                                                        // -- DEVO FERMARE IL MOVIMENTO 
            stato_APRI = 0;                                                       // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
            Serial.println("dentro STOPoooooooo");
            T_scemo_1 = millis();}
          break;         

             
  }
//------------------------------------------------------------------------------------------------------------  
 }
if (U_P == false && set == 1){
    switch (Pulsanti) {
      
      case 2010:             // -- APPENA PREMUTO APRI
        if (_Dir != 0){ ferma_WARD(); return;}                                         // -- SE SONO IN MOVIMENTO > FERMO         
        delay(50);
        if(digitalRead(APRI)!=LOW){ return;}                                           // -- ALTRIMENTI ASPETTO 300      
        T_scemo_2 = millis();                                                          // -- > GUARDO IL TEMPO
           if ((T_scemo_2 - T_scemo_1) > 100 ){                                        // -- ANTI SCEMO, SE IL TEMPO CHE PASSA DALL'ULTIMA VOLTA CHE HO RILASCIATO UN PULSANTE  E' SUPERIORE A 500..
            if (Stato_Anta[1] == "T"){                                                 // -- SE LA PORTA E' TUTTA CHIUSA (PREPARO PER L'EASY_PASS)
             while(digitalRead(APRI) == LOW){                                          // -- MI METTO IN ATTESA
              T_scemo_1 = millis();
              if ((T_scemo_1 - T_scemo_2) > 1000 ){
                solopassaggio = true;                                                  // -- SE SUPERO IL TEMPO APRO CON EASY_PASS
                Partenza_Sicura(1,2,0);
                _Dir = 1;
                // #-- Previus_Dir = _Dir;
                delay(50);
                stato_APRI = 1;
                break;
               }
              }
              if (solopassaggio == false)
                   {
                    Partenza_Sicura(1,1,0);                                            // -- APRO DA TUTTO CHIUSO SENZA EASY_PASS
                    break;
                    }
              
            }else{
            Partenza_Sicura(1,1,0);                                                    // -- APRO NORMALMENTE                                                                 
            break;
            }
           }
         stato_APRI = 1;                                                               // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
        break;
//------------------------------------------------------------------------------------------------------------    
       case 2110 :           // -- PREMUTO STABILMENTE APRI
                                                                                       // -- NON FACCIO NIENTE
          break;


//------------------------------------------------------------------------------------------------------------    
       case 1021 :           // -- PREMUTO STABILMENTE CHIUDI
                                                                                       // -- NON FACCIO NIENTE
          break;


//------------------------------------------------------------------------------------------------------------        
      case 1110 :            // -- APPENA RILASCIATO APRI
                                           
          stato_APRI = 0;                                                               // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
          T_scemo_1 = millis();
           
         break;
//------------------------------------------------------------------------------------------------------------         
      case 1020 :            // -- APPENA PREMUTO CHIUDI
          if (_Dir != 0){ferma_WARD();  stato_CHIUDI = 1; return; }                     // -- SE SONO IN MOVIMENTO > FERMO    
          delay(50); if(digitalRead(CHIUDI)!=LOW){ return;}                             // -- ALTRIMENTI ASPETTO 300              
          T_scemo_2 = millis();                                                         // -- > GUARDO IL TEMPO
          if ((T_scemo_2 - T_scemo_1) > 100 ){                                          // -- ANTI SCEMO, SE IL TEMPO CHE PASSA DALL'ULTIMA VOLTA CHE HO RILASCIATO UN PULSANTE  E' SUPERIORE A 500..
             Partenza_Sicura(-1,0,0);                                                   // -- > CHIUDE
             stato_CHIUDI = 1;                                                          // -- > IMPOSTA IL PULSANTE SU PREMUTO
          }
         break;
 //------------------------------------------------------------------------------------------------------------         
       case 1011 :          // -- APPENA RILASCIATO CHIUDI
         stato_CHIUDI = 0;                                                              // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
         T_scemo_1 = millis();
         break;                  
    }
}

switch (Pulsanti) {

//------------------------------------------------------------------------------------------------------------        
      case 2120 :           // -- premuto Apri E APPENA premuto Chiudi 
          ferma_WARD();                                                                 // -- DEVO FERMARE IL MOVIMENTO 
          stato_CHIUDI = 1;                                                             // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
        break;
//------------------------------------------------------------------------------------------------------------       
      case 2021 :           // -- premuto Chiudi E APPENA premuto Apri 
          ferma_WARD();                                                                 // -- DEVO FERMARE IL MOVIMENTO 
          stato_APRI = 1;                                                               // -- > IMPOSTA IL PULSANTE SU NON PREMUTO
         break;
//------------------------------------------------------------------------------------------------------------         
       case 2020 :          // -- APPENA premuto Apri e Chiudi     
          ferma_WARD();                                                                 // -- DEVO FERMARE IL MOVIMENTO
          stato_APRI = 1;                                                               // -- > IMPOSTA IL PULSANTE SU PREMUTO
          stato_CHIUDI = 1;                                                             // -- > IMPOSTA IL PULSANTE SU PREMUTO  
         break;
//------------------------------------------------------------------------------------------------------------         
       case 2121 :          // -- premuto stabilmente Apri e Chiudi                     
          T_scemo_2 = millis();                                                         // -- > GUARDO IL TEMPO
          while(digitalRead(APRI)==LOW && digitalRead(CHIUDI)==LOW){ 1+1;}              // -- MI METTO IN ATTESA
          T_scemo_1 = millis();
              if ((T_scemo_1 - T_scemo_2) > 3500 && (T_scemo_1 - T_scemo_2) < 10000 ){  // -- PROCEDURA PER COMMUTARE UOMO PRESENTE
                   Commuta_Uomo_Presente();                                
               }
               if ((T_scemo_1 - T_scemo_2) > 15000 ){                                   // -- PROCEDURA PER RIFARE IL set_reset_ridotto();
                   set_reset_ridotto();
               }
            stato_APRI = 0;                                                         
            stato_CHIUDI = 0; 
            delay(500);
         break;           
  } 
}
void Commuta_Uomo_Presente(){
 if ( Pos_Anta[1] > 2000 && set == 1 )
   { 
     ferma_WARD();
     delay(200);
     inVia(1, "2224", 101);
     attendi_risposta(1, 5, "2224");
     delay(300);
     ferma_WARD();
     delay(400);
     inVia(1, "2222", 102);
     attendi_risposta(1, 5, "2222");
     delay(300);
     ferma_WARD();
     U_P = !U_P;
   }
}
