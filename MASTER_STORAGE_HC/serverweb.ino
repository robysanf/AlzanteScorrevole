void myhandlenotfound() {

  if (server.uri().startsWith("/data/get/rob/")) {
    String s = server.uri().substring(14);
    server.send(200, "text/html", "<p><b>" + s + "</b><br>" + abbina_variabile(s) + "</p>" + lista_variabili_disponibili());
  }
 String strwebGet = String(server.args());
  Serial.println("ricevo : " + server.uri());
  if (server.uri().startsWith("/command")) {
    strwebHost = urldecode(server.uri().substring(9));
  }
 
  String strweb = strwebHost.substring(0, 4);
  String strweb2 = strwebHost.substring(4);
  Serial.println(strwebGet);
  if ( strweb == "apri") {                     // -- comando per APRIRE
    Partenza_Sicura(1, 0, 0);
    Serial.println("APRI");
  }
  if ( strweb == "chiu") {                    // -- comando per CHIUDERE
    Partenza_Sicura(-1, 0, 0);
    Serial.println("CHIUDI");
  }
  if ( strweb == "stop") {                    // -- comando per FERMARE
    ferma_WARD();
    Serial.println("stop");
  }
  if ( strweb == "save") {                    // -- comando per salvare
    salva_dati();
    server.send(200, "text/html", Storage_Read_Web());
  }
  if ( strweb == "load") {                    // -- comando per CARICARE LE VARIABILI DA STORAGE
    Storage_Load();
    server.send(200, "text/html","<p><a href=\"stop\">Torna al Menu</a></p>" );
  }
  
  if ( strweb == "XXXX") {                    // -- comando per CARICARE MANUALMENTE LA STRINGA
    server.send(200, "text/html", "<!DOCTYPE html>"
                                  "<html>"
                                  "<body>"
                                  "<h2>RECUPERO DI EMERGENZA</h2>"
                                  "<p>Scrivere manualmente la stringa dello storage</p>"
                                  "<form action=\"SaVe\" id=\"form1\" method=\"get\" >"
                                  "<textarea name=\"message\" rows=\"3\" cols=\"40\"></textarea>"
                                  "<br>"
                                  "<input type=\"submit\">"
                                  "</form>"
                                  "</body>"
                                  "</html>");
  }
  if ( strweb == "SaVe") {                    // -- comando per CARICARE MANUALMENTE LA STRINGA
    salva_dati_emergenza();
    server.send(200, "text/html", "<!DOCTYPE html>"
                                  "<html>"
                                  "<body>"
                                  "<h2>RECUPERO DI EMERGENZA EFFETTUATO</h2><br>"
                                  "<p><a href=\"stop\">Torna al Menu</a></p>"
                                  "</body>"
                                  "</html>");
  }
  if ( strweb == "setr") {                    // -- comando per CARICARE rifare il set-reset
    set = 0;
    chiedo_conf = true;
    server.send(200, "text/html", "<!DOCTYPE html>"
                                  "<html>"
                                  "<body>"
                                  "<p><a href=\"stop\">Torna al Menu</a></p>"
                                  "</body>"
                                  "</html>");
  }
  if ( strweb == "vars") {                    // -- comando per VEDERE LE VARIABILI
    //Stop(5);
    Serial.println("vars");
    //strwebHostOld = strwebHost;
    //time_strwebHostOld = millis();
    //String Da_Stampare = Storage_Read_Web();
    server.send(200, "text/html", Storage_Read_Web());
  }else{
    server.send(200, "text/html", "<p><a href=\"apri\">Apri</a></p>"
                                  "<p><a href=\"stop\">Stop</a></p>"
                                  "<p><a href=\"chiu\">Chiudi</a></p>"
                                  "<p><a href=\"vars\">leggi le variabili</a></p>"
                                  "</p><a href=\"load\">ricarica le variabili</a></p>"
                                  "</p><a href=\"setr\">set-reset</a></p>"
                                  "</p><a href=\"XXXX\">RIPARA</a></p>");
  }
}
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

String abbina_variabile(String trova) {
  if (trova == "_Pulsanti") {
    return String(_Pulsanti);
  }
  if (trova == "imp") {
    return String(imp);
  }
  if (trova == "Batteria_Tensione") {
    String risposta;
    for ( int i = 1; i <= Numero_Ante_Slave  ; i = i + 1) {
      risposta += Batteria_Tensione[i] + "<br>" ;
    }
    return risposta;
  }
  if (trova == "My_Buffer") {
    String risposta;
    for ( int i = 1; i <= Numero_Ante_Slave  ; i = i + 1) {
      risposta += My_Buffer[i] + "<br>" ;
    }
    return risposta;
  }
  if (trova == "cicli_test") {
    return String(cicli_test);
  }
  if (trova == "set") {
    return String(set);
  }
  if (trova == "addr_scanner") {
    return addrg;
  }
  if (trova == "error") {
    return error;
  }
  if (trova == "") {
    return "<b>Scegli cosa vedere</b>";
  }
  return "<b>non trovato</b>";
}

String lista_variabili_disponibili() {
  String da_scrivere = "";
  da_scrivere = "<p>";
  da_scrivere = da_scrivere + "<a href=\"imp\">imp</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"My_Buffer\">My_Buffer</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"cicli_test\">cicli_test</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"_Pulsanti\">_Pulsanti</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"addr_scanner\">addr_scanner</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"set\">set</a><b> -- </b>";
  da_scrivere = da_scrivere + "</p>";
  return da_scrivere;
}
