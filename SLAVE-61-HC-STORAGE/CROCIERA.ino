boolean crocera(int verso) { 
  velocita_misurata = Tachi_Metro();
  //VELOCITA_CROCERA * ( 1 + 0,05 - ( VELOCITA_CROCERA * 0,01 / 100 ) )
 Serial.print("Vel corcera :");Serial.println(velocita_misurata); 
 Serial.print("Vel target :");Serial.println(velocita_crocera); 
 //if ( (verso == 1 && velocita_misurata > (velocita_crocera * (1 - V_Range_UP + ( velocita_crocera * 0.01 / 100 )))) || (verso == -1 && velocita_misurata < (velocita_crocera * (1 + V_Range_Down - ( velocita_crocera * 0.01 / 100 ))))){
if ( (verso == 1 && velocita_misurata > (velocita_crocera * (1 - V_Range_UP))) || (verso == -1 && velocita_misurata < (velocita_crocera * (1 + V_Range_Down )))){
    return true;
  }
  else {
    return false;
  }
}
