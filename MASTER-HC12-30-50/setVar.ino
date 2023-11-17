void setVar(){
      /* tabella parametri sullo slave
      3331: // -- PER CAMBIARE LA VELOCITA' MASSIMA
      3332: // -- PER CAMBIARE LA VELOCITA' MINIMA
      3333: // -- PER CAMBIARE LA VELOCITA' IN COLLISIONE  OPEEN CLOSE FAAST SETT1
      3334: // -- PER CAMBIARE LA DIREZIONE DEL MOTORE
      3336: // -- PER CAMBIARE IL VALORE DI ATTRITO
      3337: // -- E' LIBERO
      3338: // -- PER CAMBIARE IL VALORE DI INERZIA
      3339: // -- PER CAMBIARE IL VALORE DI PESO
      3310: // -- PER SETTARE IL VALORE DI POS_APERTO
   */
   int temporaneo = 0;
   String temporaneo2 = "";
    inviaTutti("3311", String(imp*100));
    delay(10);
    velocita_crocera_MAX=dataRead("velocita_crocera_MAX").toInt()*1.00;
   
    delay(10);
    Serial.print("\n Velocita max da file = "+String(velocita_crocera_MAX));
    velocita_crocera_LENTA=dataRead("velocita_crocera_LENTA").toInt()*1.00;
  Serial.print("\n Velocita crocere lenta da file = "+String(velocita_crocera_LENTA));
    delay(10);
   // Serial.print("\n Velocita rocera COLL = "+String(velocita_crocera_LENTA));
    String velocita_crocera_COL=dataRead("velocita_crocera_COL");
    delay(10);
  //  Serial.print("\n Ricevi7 = "+String(velocita_crocera_COL));
    inviaTutti("3333", String(velocita_crocera_COL.toInt()*100));
 /*   for (int i = 1; i<= Numero_Ante_Slave; i = i +1){
      temporaneo = velocita_crocera_COL.toInt()*100;
      temporaneo2 = "3333"; 
      temporaneo2 += temporaneo;
      inVia(i,temporaneo2);// -- DA IPOSTARE 1L 333
    }*/
    delay(10);
    String attrito_arr=dataRead("attrito");
    
     Serial.print("\n attrito letto  = " + attrito_arr);
    delay(10);
    parseVar2( ATTRITO , attrito_arr);
    inviaOgnunoString("3336", attrito);
       delay(10);
    temporaneo=0;
  //  Serial.print("\n attrito letto  = " ); Serial.print(String(attrito[1]));Serial.print(" - "); Serial.print(attrito[2]);
  Serial.print("\n Numero_Ante_Slave  = " );Serial.println(Numero_Ante_Slave);
 /*   for (int i = 1; i<= Numero_Ante_Slave; i = i +1){
       Serial.print("\n attrito array  = ");Serial.print(String(attrito[i])); 
      temporaneo = String(attrito[i]).toInt()*100;
      Serial.print("\n attrito calcolato  = " + String(temporaneo));
      temporaneo2 = "3336"; 
      temporaneo2 += temporaneo;
      inVia(i,temporaneo2);// -- DA IPOSTARE 1L 333
    }*/
    delay(10);
    String motore_arr=dataRead("motore");
    delay(10);
    parseVar2( MOTORE , motore_arr);
     inviaOgnunoString("3334", motore);
  /*  for (int i = 1; i<= Numero_Ante_Slave; i = i +1){
      if ( (String)motore[i]== "+" ){temporaneo = 1;} else {temporaneo = -1;}
      temporaneo2 = "3334"; 
      temporaneo2 += temporaneo;
      inVia(i,temporaneo2);
    }*/
   
   // ordina_Web("gettencoder/");
   String encoder_arr=dataRead("encoder");
    delay(10);
    parseVar2( ENCODER ,encoder_arr);
     inviaOgnunoString("3335", encoder);
    
/*    for (int i = 1; i<= Numero_Ante_Slave; i = i +1){
      if ( (String)encoder[i] == "+" )
      {temporaneo = 1;} 
      else {temporaneo = -1;}
      temporaneo2 = "3335"; 
      temporaneo2 += temporaneo;
       inVia(i,temporaneo2);
    }*/
    delay(10);
    String ritardo_arr=dataRead("ritardo");
    delay(10);
    parseVar2( RITARDO , ritardo_arr);                      
   /* for (int i = 1; i<= Numero_Ante_Slave; i = i +1){
      Serial.print("\n ritardo [");Serial.print(i);Serial.print("] = ");Serial.print(String(ritardo[i]));
    }*/
    String inerzia_arr=dataRead("inerzia");
    delay(10);
    parseVar2( INERZIA ,inerzia_arr);
    inviaOgnunoInt("3338", inerzia);
   /* for (int i = 1; i<= Numero_Ante_Slave; i = i +1){
      temporaneo = inerzia[i];
      temporaneo2 = "3338"; 
      temporaneo2 += temporaneo;
      inVia(i,temporaneo2);
    }*/
    delay(10);
    String peso_arr=dataRead("peso");
     parseVar2( PESO , peso_arr);
      inviaOgnunoInt("3339", peso);
   /* for (int i = 1; i<= Numero_Ante_Slave; i = i +1){
      temporaneo = peso[i];
      temporaneo2 = "3339"; 
      temporaneo2 += temporaneo;
       inVia(i,temporaneo2);
    }*/ 
    delay(10);
    String pos_aperto_arr=dataRead("pos_aperto");
    
    parseVar2( POS_APERTO ,pos_aperto_arr);
     inviaOgnunoLong("3310", pos_aperto);
      Per_Centuale = (pos_aperto[1] - (Distanza*((Numero_Ante_Slave+1)-1)))/100;
  /*  for (int i = 1; i <= Numero_Ante_Slave; i = i + 1){
    //  Serial.print("\n posaperto parsed ");Serial.print(pos_aperto[i]);
      long temporaneo = (pos_aperto[i] - (Distanza*((Numero_Ante_Slave+1)-i)));
      if (i==1){
        Per_Centuale = (pos_aperto[i] - (Distanza*((Numero_Ante_Slave+1)-i)))/100;
       // Serial.print("\n ****** Per_Centuale  =");Serial.print(Per_Centuale);
      }
      
      temporaneo2 = "3310"; temporaneo2 += temporaneo;
     inVia(i,temporaneo2);
    }*/
    delay(10);

    // -- ORA POSSO PASSARE AGLI SLAVE LA VELOCITA
    calcola_velocita();
  
  }
