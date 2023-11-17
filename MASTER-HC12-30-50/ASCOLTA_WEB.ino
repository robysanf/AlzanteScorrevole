void ascolta_web() {
    if ( millis() - time_strwebHostOld > 2500){
      strwebHostOld = "0000";
    }
    delay(wait);
    if (strwebHost != strwebHostOld ) {
    strweb = strwebHost.substring(0, 4);
    strweb2 = strwebHost.substring(4);
    if ( strweb == "setr") {                    // -- comando per SET_RESET
      int pos_mot = strweb2.indexOf("#");
      int pos_enc = strweb2.indexOf("#", pos_mot + 1);
      String mot = strweb2.substring(pos_mot + 1, pos_enc);
      String enc = strweb2.substring(pos_enc + 1); // salvo già gli encoder e motore perchè tanto se arrivo qui sono già corretti
      //writelog("motore e enc ## " + mot + " ## " + enc);
      dataSave("encoder", enc);
      dataSave("motore", mot);
      set_reset();
      strwebHostOld = strwebHost;
    }
    if ( strweb == "pos0") {                    // -- comando per SET zero pos
     // writelog("pos0");
      for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
        inVia(i, "2226",45); delay(500);
        strwebHostOld = strwebHost;
      }
    }
    if ( strweb == "save") {
      comando = "9999";                           // -- comando per SET_RESET
    }
    if ( strweb == "scan") {
      comando = "2221";                          // -- comando per SCAN
    }
    if ( strweb == "move") {                     // -- comando per MOVE

      sposta(strweb2);
      strwebHostOld = strwebHost;
    }
    if ( strweb == "apri") {                     // -- comando per APRIRE
      Partenza_Sicura(1, 0, 0);
      Serial.println("APRI");
      strwebHostOld = strwebHost;
      time_strwebHostOld = millis();
    } 
    if ( strweb == "chiu") {                    // -- comando per CHIUDERE
      Partenza_Sicura(-1, 0, 0);
      Serial.println("CHIUDI");
      strwebHostOld = strwebHost;
      time_strwebHostOld = millis();
    }
    if ( strweb == "stop") {                    // -- comando per FERMARE
      ferma_WARD(); 
      Serial.println("stop");
      strwebHostOld = strwebHost;
      time_strwebHostOld = millis();
    }
    if ( strweb == "chch") {                    // -- comando per FERMARE
      
      Serial.println("cambia canale");
       for (int i = 1 ; i <= Numero_Ante_Slave ; i++) {
        inVia(i, "5555"+strweb2,45); 
        attendi_risposta(i, 10, "5555");
        strwebHostOld = strwebHost;
      }
      digitalWrite(3, LOW );
      delay(150);
      String s="AT+C"+strweb2;
      char lis[255];
      s.toCharArray(lis,255);
      Serial1.write(lis);
      delay(150);
      digitalWrite(3, HIGH );
     
    }
    if ( strweb == "comu") {
      comando = "4441";                         // -- comando per COMMUTA UOMO PRESENTE
    }
    if ( strweb == "easy") {
      comando = "2224";                         // -- comando per EASY_PASS
    }
    if ( strweb == "limi") {
      comando = "1120";                         // -- comando per eseguire il controllo del limit
     // inviaTutti("1116","");
    }
    if ( strweb == "impe") {
      comando = strweb2 ;
      save(strweb2);
    }  // -- comando per micro aereazione ( con zanzariera prossimo anno)d = ");Serial.print(strweb_hold);
    delay(1);
    strwebHost = "0000";
    //-@-  Serial.println("imposto il strwebHostOld = " + String(strwebHostOld));
    delay(1);
  }
}

void save(String s) {                           // -- solo per velocità max e lenta
  //writelog("cambio velocita " + s);
  String val = s.substring(4);
  if (s.startsWith("3331")) {
    dataSave("velocita_crocera_MAX", val);
  }
  if (s.startsWith("3332")) {
    dataSave("velocita_crocera_LENTA", val);
  }
  if (s.startsWith("3333")) {
    dataSave("velocita_crocera_COL", val);
  }
  if (s.startsWith("4333")) {
   
    String str = s.substring(4);
    //writelog("prendo solo " + str);
    char list[32];
    str.toCharArray(list, 32);
    //int pos_num = 0;
    char * pch;
    String toret[6] = {"", "", "", "", "", ""};
    pch = strtok (list, ",");
    int cont = 0;
    while (pch != NULL)
    {
      toret[cont] = pch;
      cont++;

      pch = strtok (NULL, ",");
    }
    //writelog("aray " + toret[0] + " " + toret[1] + " " + toret[2] + " " + toret[3] + " " + toret[4]);
    char d[6];
    toret[0].toCharArray(d, 6);
    int pos_n = atoi(d);
    //writelog("door " + String(pos_n));
    aggiornaVar("attrito", toret[1], pos_n);
    aggiornaVar("inerzia", toret[2], pos_n);
    aggiornaVar("peso", toret[3], pos_n);
    aggiornaVar("ritardo", toret[4], pos_n);
    //writelog(toret[1] + "-" + toret[2] + "-" + toret[3] + "-" + toret[4]);

  }

}

void aggiornaVar(String nome_var , String nuovo_val, int pos_num) {
  String att = dataFiller(nome_var);
  if (att.indexOf(",") == -1) {
    dataSave(nome_var, nuovo_val + "/");
  }
  char list[30];
  char nuovo_valore[5];
  att.toCharArray(list, 30);
  nuovo_val.toCharArray(nuovo_valore, 70);
  char * pch;
  String var_to_return;
  char toret[40] = "";
  pch = strtok (list, ",");
  int cont = 0;
  while (pch != NULL)
  {
    if (cont == pos_num)
    {
      strcat(toret, nuovo_valore);
    }
    else {
      strcat(toret, pch);
    }
    strcat(toret, ",");
    cont++;

    pch = strtok (NULL, ",");
  }
  String tosave = String(toret);
  tosave = tosave.substring(0, tosave.length() - 1); //tolgo ultima virgola
  tosave.replace("/", "");
  tosave.replace("\r", "");
  tosave.replace("\n", "");
  //writelog("tosave " + tosave);
  dataSave(nome_var, tosave);
}
