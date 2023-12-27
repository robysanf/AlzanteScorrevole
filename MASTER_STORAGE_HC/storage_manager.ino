String Storage_Load() {
  WiFiStorageFile file = WiFiStorage.open("/fs/config");
  String s = "";
  uint8_t buf[STORAGE_BUFFER];
  char* list0="\0";
  if (file)
  {
    int dim=file.size();
    file.seek(0); 
   Serial.print("dim ");Serial.println(dim);
     if(dim>250){
        file.read(buf, STORAGE_BUFFER);
        uint8_t buf2[dim-STORAGE_BUFFER-1];
        file.seek(250);
        file.read(buf2, dim-STORAGE_BUFFER-1);
        Serial.print("buf ");Serial.println((char*) buf);
        Serial.print("buf2 ");Serial.println((char*) buf2);
        list0=strcat((char*) buf,(char*) buf2);
     }else
     {
       file.read(buf, dim);
       Serial.print("buf ");Serial.println((char*) buf);
       list0=(char*) buf;
      }
     Serial.print("Concat ");Serial.println(list0);
     
  }
  file.close();
   Serial.print("leggo Storage = "); Serial.println(list0);
  char * pch;
  pch = strtok (list0, "#");
  Serial.print(" prima conversione per spaccare in # "); Serial.println(pch);
  String array_var[30];
  int cont = 0;
  while (pch != NULL)
  {
    String s = String(pch);
    Serial.print("valore che passo al spaccaChiaveValore : "); Serial.println(s);
    array_var[cont] = s;
    pch = strtok (NULL, "#");
    cont++;
  }
  for (int i = 0; i < sizeof(array_var); i++)
  { 
    if (array_var[i] == "")break;
    spaccaChiaveValore(array_var[i]);
  }
  return s;
}
String Storage_Load_1() {
  WiFiStorageFile file = WiFiStorage.open("/fs/config");
  String s = "";
  uint8_t buf[STORAGE_BUFFER];
   uint8_t buf2[250];
  if (file)
  {
    file.seek(0);
    file.read(buf, 250);
    Serial.println((char*) buf);
    file.seek(250);
    file.read(buf2, 500);
     Serial.println(strcat((char*) buf,(char*) buf2));
  }
  file.close();
  char* list0 = strcat((char*) buf,(char*) buf2);
  Serial.print("leggo Storage = "); Serial.println(list0);
  char * pch;
  pch = strtok (list0, "#");
  Serial.print(" prima conversione per spaccare in # "); Serial.println(pch);
  String array_var[30];
  int cont = 0;
  while (pch != NULL)
  {
    String s = String(pch);
    Serial.print("valore che passo al spaccaChiaveValore : "); Serial.println(s);
    array_var[cont] = s;
    pch = strtok (NULL, "#");
    cont++;
  }
  for (int i = 0; i < sizeof(array_var); i++)
  { 
    if (array_var[i] == "")break;
    spaccaChiaveValore(array_var[i]);
  }
  return s;
}
void spaccaChiaveValore(String str_line) {
  Serial.print(" str_line: "); Serial.println(str_line);
  int posiz = str_line.indexOf("=");
  if (posiz > 0)
  {
    char var_str[50];
    str_line.substring(0, posiz).toCharArray(var_str, 50);
    String valore = str_line.substring(posiz + 1);
    int j;
    for (j = 0;  j < sizeof (conversion) / sizeof (conversion[0]);  ++j)
    {
      if (!strcmp (var_str, conversion[j].str))
      {
        switch (conversion[j].val)
        {
          case DIPENDENTE:               dipendente = String(valore); break;                            // @-- Serial.print("dipendente = "); Serial.println(dipendente); break;
          case NUMERO_ANTE_SLAVE:        Numero_Ante_Slave = String(valore).toInt(); break;
          case VELOCITA_CROCERA_MAX:     velocita_crocera_MAX = String(valore).toFloat(); break;
          case VELOCITA_CROCERA_LENTA:   velocita_crocera_LENTA = String(valore).toFloat(); break;
          case ANTE_SLAVE:               stringaToIntArray(valore, Ante_Slave); break;                 // @-- Serial.print(" Ante_Slave pch: "); Serial.println(Ante_Slave[cont])
          case RITARDO:                  stringaToFloatArray(valore, ritardo); break;
          case ATTRITO:                  stringaToStringArray(valore, attrito); break;                         // @-- Serial.print("\n attrito pch: "); Serial.print(attrito[cont]);
          case MOTORE:                   stringaToStringArray(valore, motore); break;
          case ENCODER:                  stringaToStringArray(valore, encoder); break;
          case INERZIA:                  stringaToIntArray(valore, inerzia); break;
          case PESO:                     stringaToIntArray(valore, peso); break;
          case ADDR:                     stringaToIntArray(valore, addr); break;                          // @-- Serial.print("\n addr= "); Serial.print(String( addr[cont]));
          case POS_APERTO:               stringaToLongArray(valore, pos_aperto); break;
          case IMP:                      imp = String(valore).toFloat(); break;
          case POS_CHIUSO:               pos_chiuso = String(valore).toInt(); break;
          case CONFIGURAZIONE:           configurazione = String(valore); break;
          case POS:                      //pos = String(valore).toInt(); break;
          case CADENZA:                  cadenza = String(valore).toInt(); break;
          case VELOCITA_CROCERA_COL:     velocita_crocera_COL = String(valore).toFloat(); break;
        }
        break;
      }
    }
  }
}

void stringaToFloatArray(String str, float var_array[]) {
  char list[50];
  str.toCharArray(list, 50);
  char * pch;
  pch = strtok (list, ",");
  int cont = 1;
  while (pch != NULL)
  {
    String s = String(pch);
    var_array[cont] = s.toInt();
    pch = strtok (NULL, ",");
    cont++;
  }
}
void stringaToLongArray(String str, long var_array[]) {
  char list[50];
  str.toCharArray(list, 50);
  char * pch;
  pch = strtok (list, ",");
  int cont = 1;
  while (pch != NULL)
  {
    String s = String(pch);
    var_array[cont] = s.toInt();
    pch = strtok (NULL, ",");
    cont++;
  }
}

void stringaToIntArray(String str, int var_array[]) {
  char list[50];
  str.toCharArray(list, 50);
  char * pch;
  pch = strtok (list, ",");
  int  cont = 1;
  while (pch != NULL)
  {
    String s = String(pch);
    var_array[cont] = s.toInt();
    pch = strtok (NULL, ",");
    cont++;
  }
}

void stringaToStringArray(String str, String var_array[]) {
  char list[50];
  str.toCharArray(list, 50);
  char * pch;
  pch = strtok (list, ",");
  int  cont = 1;
  while (pch != NULL)
  {
    String s = String(pch);
    var_array[cont] = s;
    pch = strtok (NULL, ",");
    cont++;
  }
}

String Storage_Read_Web() {
  WiFiStorageFile file = WiFiStorage.open("/fs/config");
  String stampa = "<!DOCTYPE html>"
                  "<html>"
                  "<body>"
                  "<b>IMPOSTAZIONI PRIMARIE</b><br>"
                  "Modificare le seguenti variabili puo causare un mal funzionamento<br>"
                  "<form action=\"save\" id=\"form1\" method=\"get\" >"
                  "<table><tr>";
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
  stampa += "</tr></table>"
            "<input type=\"submit\"  value=\"MODIFICA E SALVA\" target=\"_blank\">"
            "</form>"
            "<p><a href=\"stop\">Torna al Menu</a></p>"
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
    String variabile = "";
    for (j = 0;  j < sizeof (conversion) / sizeof (conversion[0]);  ++j)
    {
      if (!strcmp (var_str, conversion[j].str))
      { variabile = String(conversion[j].str);
        switch (conversion[j].val)




        { //******  le varibili qui devono essere le stesse dell enum nelle varibili.h
          case RAGGIO:               Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case IMP:                  Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case POS_APERTO:           Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case POS_CHIUSO:           Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case VELOCITA_BASSA:       Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case VELOCITA_MEDIA:       Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case VELOCITA_ALTA:        Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case MOTORE:               Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case ENCODER:              Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case CONFIGURAZIONE:       Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case POS:                  Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case CADENZA:              Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case CONSUMO_BASSA_MAX:    Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case CONSUMO_MEDIA_MAX:    Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case CONSUMO_ALTA_MAX:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case DIPENDENTE:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                                               "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case NUMERO_ANTE_SLAVE:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case VELOCITA_CROCERA_MAX:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case VELOCITA_CROCERA_LENTA:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case VELOCITA_CROCERA_COL:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case ANTE_SLAVE:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                                               "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case RITARDO:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                                            "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case ATTRITO:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                                            "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case INERZIA:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                                            "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case PESO:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                                         "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
            break;
          case ADDR:     Da_Stampare = "<td><label for=\"" + variabile + "\">" + variabile + " :</label></td>"
                                         "<td><input type=\"text\" id=\"" + variabile + "\" name=\"" + variabile + "\" value=\"" + valore + "\"></td></tr>";
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
void salva_dati() {

  String message = "";

  for (int i = 0; i < server.args(); i++) {


    message += server.argName(i) + "=";            //Get the name of the parameter
    message += server.arg(i) + "#";              //Get the value of the parameter


  }
  message = message.substring(0, message.length() - 1);
  Storage_SaveAll(message);

}
void salva_dati_emergenza() {

  String message = "";
  message += server.arg(0);
  message += "@";
  Storage_SaveAll(message);

}
