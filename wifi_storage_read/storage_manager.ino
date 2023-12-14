String Storage_Load() {
  WiFiStorageFile file = WiFiStorage.open("/fs/config");
  String s = "";
  uint8_t buf[STORAGE_BUFFER];
  if (file)
  {
    file.seek(0);
    file.read(buf, STORAGE_BUFFER);
    //s = String((char*) buf);
    //Serial.print("stringa = ");Serial.println(s);
  }
  file.close();
  char* list = (char*) buf;
  Serial.println("leggo load:");
  Serial.println(list);
  char * pch;
  pch = strtok (list, "#");

  while (pch != NULL)
  {
    String s = String(pch);
    //Serial.print("string s: ");Serial.println(s);
    spaccaChiaveValore(s);
    pch = strtok (NULL, "#");
    //Serial.print("in while: ");Serial.println(pch);
  }
  return s;
}


void spaccaChiaveValore(String str_line) {
  Serial.print(" str_line: "); Serial.println(str_line);
  int pos = str_line.indexOf("=");
  if (pos > 0)
  {
    char var_str[50];
    str_line.substring(0, pos).toCharArray(var_str, 50);
    String valore = str_line.substring(pos + 1);
    Serial.print("var_str "); Serial.println(var_str);
    int j;
    for (j = 0;  j < sizeof (conversion) / sizeof (conversion[0]);  ++j)
       { //Serial.println("for:");
      //Serial.println(var_str);Serial.println(conversion[j].str);
      if (!strcmp (var_str, conversion[j].str))
      {
        switch (conversion[j].val)
          //switch (var_str)
          { //******  le varibili qui devono essere le stesse dell enum nelle varibili.h
            case RAGGIO:               raggio = String(valore).toFloat(); Serial.print("set raggio:"); Serial.println(raggio); break;
            case IMP:                  imp = String(valore).toFloat(); Serial.print("set imp:"); Serial.println(imp); break;
            case POS_APERTO:           pos_aperto = String(valore).toInt(); break;
            case POS_CHIUSO:           pos_chiuso = String(valore).toInt(); break;
            case VELOCITA_BASSA:       Velocita_Bassa = String(valore).toFloat(); ; break;
            case VELOCITA_MEDIA:       Velocita_Media = String(valore).toFloat(); break;
            case VELOCITA_ALTA:        Velocita_Alta = String(valore).toFloat(); break;
            case MOTORE:               motore = String(valore).toInt(); break;
            case ENCODER:              encoder = String(valore).toInt(); Serial.print("set encoder:"); Serial.println(encoder); break;
            case CONFIGURAZIONE:       configurazione = String(valore).toInt(); break;
            case POS:                  //pos = String(valore).toInt(); break;
            case CADENZA:              cadenza = String(valore).toInt(); break;
            case CONSUMO_BASSA_MAX:    consumo_Bassa_max = String(valore).toInt(); break;
            case CONSUMO_MEDIA_MAX:    consumo_Media_max = String(valore).toInt(); break;
            case CONSUMO_ALTA_MAX:     consumo_Alta_max = String(valore).toInt(); break;
            default: break;
          }
        break;
      }
  }
}

}

String Storage_Read_Web() {
  WiFiStorageFile file = WiFiStorage.open("/fs/config");
  String stampa = "<!DOCTYPE html>"
                  "<html>"
                  "<body>"
                  "<h1>IMPOSTAZIONI PRIMARIE</h1>"
                  "<p>Modificare le seguenti variabili puo causare un mal funzionamento</p>"
                  "<form action=\"save\" id=\"form1\" method=\"get\" >";
  uint8_t buf[STORAGE_BUFFER];
  if (file)
  {
    file.seek(0);
    file.read(buf, STORAGE_BUFFER);
    //s = String((char*) buf);
    //Serial.print("stringa = ");Serial.println(s);
  }
  file.close();
  char* list = (char*) buf;
  //Serial.println("leggo load:");
  //Serial.println(list);
  char * pch;
  pch = strtok (list, "#");
  Serial.print("string pch: "); Serial.println(String(pch));
  while (pch != NULL)
  {
    String s = String(pch);
    //Serial.print("string s: ");Serial.println(s);
    stampa += stampa_storage_web(s);
    pch = strtok (NULL, "#");
    //Serial.print("in while: ");Serial.println(pch);
    delay(1);
  }
  stampa += "<input type=\"submit\"  value=\"MODIFICA E SALVA\" target=\"_blank\">"
            "</form>"
            "</body>"
            "</html>\0";


  //Serial.print("da stampare = ");Serial.println( stampa);
  return stampa;
}
String stampa_storage_web(String str_line) {
  String Da_Stampare = "";
  //Serial.print(" str_line: ");Serial.println(str_line);
  int Posizione = str_line.indexOf("=");
  if (Posizione > 0)
  {
    char var_str[50];
    str_line.substring(0, Posizione).toCharArray(var_str, 50);
    String valore = str_line.substring(Posizione + 1);
    //Serial.print("var_str ");Serial.println(var_str);
    int j;

    for (j = 0;  j < sizeof (conversion) / sizeof (conversion[0]);  ++j)
    { //Serial.println("for:");
      //Serial.println(var_str);Serial.println(conversion[j].str);
      if (!strcmp (var_str, conversion[j].str))
      {
        switch (conversion[j].val)
        { //******  le varibili qui devono essere le stesse dell enum nelle varibili.h
          case RAGGIO:               Da_Stampare = "<label for=\"RAGGIO\">raggio :</label><br>"
                "<input type=\"text\" id=\"RAGGIO\" name=\"RAGGIO\" value=\"" + valore + "\"><br><br>";
            break;
          case IMP:                  Da_Stampare = "<label for=\"IMP\">imp :</label><br>"
                "<input type=\"text\" id=\"IMP\" name=\"IMP\" value=\"" + valore + "\"><br><br>";
            break;
          case POS_APERTO:           Da_Stampare = "<label for=\"POS_APERTO\">pos_aperto :</label><br>"
                "<input type=\"text\" id=\"POS_APERTO\" name=\"POS_APERTO\" value=\"" + valore + "\"><br><br>";
            break;
          case POS_CHIUSO:           Da_Stampare = "<label for=\"POS_CHIUSO\">pos_chiuso :</label><br>"
                "<input type=\"text\" id=\"POS_CHIUSO\" name=\"POS_CHIUSO\" value=\"" + valore + "\"><br><br>";
            break;
          case VELOCITA_BASSA:       Da_Stampare = "<label for=\"VELOCITA_BASSA\">Velocita_Bassa :</label><br>"
                "<input type=\"text\" id=\"VELOCITA_BASSA\" name=\"VELOCITA_BASSA\" value=\"" + valore + "\"><br><br>";
            break;
          case VELOCITA_MEDIA:       Da_Stampare = "<label for=\"VELOCITA_MEDIA\">Velocita_Media :</label><br>"
                "<input type=\"text\" id=\"VELOCITA_MEDIA\" name=\"VELOCITA_MEDIA\" value=\"" + valore + "\"><br><br>";
            break;
          case VELOCITA_ALTA:        Da_Stampare = "<label for=\"VELOCITA_ALTA\">Velocita_Alta :</label><br>"
                "<input type=\"text\" id=\"VELOCITA_ALTA\" name=\"VELOCITA_ALTA\" value=\"" + valore + "\"><br><br>";
            break;
          case MOTORE:               Da_Stampare = "<label for=\"MOTORE\">motore :</label><br>"
                "<input type=\"text\" id=\"MOTORE\" name=\"MOTORE\" value=\"" + valore + "\"><br><br>";
            break;
          case ENCODER:               Da_Stampare = "<label for=\"ENCODER\">encoder :</label><br>"
                "<input type=\"text\" id=\"ENCODER\" name=\"ENCODER\" value=\"" + valore + "\"><br><br>";
            break;
          case CONFIGURAZIONE:       Da_Stampare = "<label for=\"CONFIGURAZIONE\">configurazione :</label><br>"
                "<input type=\"text\" id=\"CONFIGURAZIONE\" name=\"CONFIGURAZIONE\" value=\"" + valore + "\"><br><br>";
            break;
          case POS:                  Da_Stampare = "<label for=\"POS\">pos :</label><br>"
                "<b> non modificabile >>  " + String(pos) + "</b><br><br>";
            break;
          case CADENZA:              Da_Stampare = "<label for=\"CADENZA\">cadenza :</label><br>"
                "<input type=\"text\" id=\"CADENZA\" name=\"CADENZA\" value=\"" + valore + "\"><br><br>";
            break;
          case CONSUMO_BASSA_MAX:    Da_Stampare = "<label for=\"CONSUMO_BASSA_MAX\">consumo_Bassa_max :</label><br>"
                "<input type=\"text\" id=\"CONSUMO_BASSA_MAX\" name=\"CONSUMO_BASSA_MAX\" value=\"" + valore + "\"><br><br>";
            break;
          case CONSUMO_MEDIA_MAX:    Da_Stampare = "<label for=\"CONSUMO_MEDIA_MAX\">consumo_Media_max :</label><br>"
                "<input type=\"text\" id=\"CONSUMO_MEDIA_MAX\" name=\"CONSUMO_MEDIA_MAX\" value=\"" + valore + "\"><br><br>";
            break;
          case CONSUMO_ALTA_MAX:     Da_Stampare = "<label for=\"CONSUMO_ALTA_MAX\">consumo_Alta_max :</label><br>"
                "<input type=\"text\" id=\"CONSUMO_ALTA_MAX\" name=\"CONSUMO_ALTA_MAX\" value=\"" + valore + "\"><br><br>";
            break;
          default: break;
        }
        break;
      }
    }
  }
  Serial.println(Da_Stampare);
  return Da_Stampare;
}


void Storage_SaveAll(String value) {
  WiFiStorageFile file = WiFiStorage.open("/fs/config");
  if (file)
    file.erase();
  file.write(value.c_str(), value.length());
  file.close();
}
