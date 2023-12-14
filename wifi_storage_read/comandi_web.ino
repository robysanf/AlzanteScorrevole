String urldecode(String str)
{

  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == '+') {
      encodedString += ' ';
    } else if (c == '%') {
      i++;
      code0 = str.charAt(i);
      i++;
      code1 = str.charAt(i);
      c = (h2int(code0) << 4) | h2int(code1);
      encodedString += c;
    } else {
      encodedString += c;
    }
    yield();
  }
  return encodedString;
}

unsigned char h2int(char c)
{
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

void handleNotFound()
{
  myhandlenotfound();
}

void myhandlenotfound() {
 String strwebGet = String(server.args());
  Serial.println("ricevo : " + server.uri());
  if (server.uri().startsWith("/command")) {
    strwebHost = urldecode(server.uri().substring(9));
  }
 
  String strweb = strwebHost.substring(0, 4);
  String strweb2 = strwebHost.substring(4);
  Serial.println(strwebGet);
  if ( strweb == "apri") {                     // -- comando per APRIRE
    //Partenza_Sicura(1, 0, 0);
    Serial.println("APRI");
    //strwebHostOld = strwebHost;
    //time_strwebHostOld = millis();
  }
  if ( strweb == "chiu") {                    // -- comando per CHIUDERE
    //Partenza_Sicura(-1, 0, 0);
    Serial.println("CHIUDI");
    //strwebHostOld = strwebHost;
    //time_strwebHostOld = millis();
  }
  if ( strweb == "stop") {                    // -- comando per FERMARE
    //Stop(5);
    Serial.println("stop");
    //strwebHostOld = strwebHost;
    //time_strwebHostOld = millis();
  }
  if ( strweb == "save") {                    // -- comando per FERMARE
    //Stop(5);
    //Serial.println(strwebGet);
    //strwebHostOld = strwebHost;
    //time_strwebHostOld = millis();
    //server.send(200, "text/html", strwebGet);
    salva_dati();
    server.send(200, "text/html", Storage_Read_Web());
  }
  if ( strweb == "vars") {                    // -- comando per VEDERE LE VASIABILI
    //Stop(5);
    Serial.println("vars");
    //strwebHostOld = strwebHost;
    //time_strwebHostOld = millis();

    Serial.println(String(pos));
    //String Da_Stampare = Storage_Read_Web();
    server.send(200, "text/html", Storage_Read_Web());
  }else{
    server.send(200, "text/html", "<p><a href=\"apri\">Apri</a></p><p><a href=\"stop\">Stop</a></p><p><a href=\"chiu\">Chiudi</a></p><a href=\"vars\">leggi le variabili</a></p>");
  }
  
}
