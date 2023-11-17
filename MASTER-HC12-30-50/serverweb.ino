void myhandlenotfound() {
  Serial.println("ricevo : " + server.uri());
  if (server.uri().startsWith("/delfile")) {
    String fil_to_del = urldecode(server.uri().substring(8));
    //Serial.println(fil_to_del);
    SD.remove(fil_to_del);
    server.send(200, "text/html", "Rimosso");
  }
  if (server.uri().startsWith("/command")) {
    strwebHost = urldecode(server.uri().substring(9));

    server.send(200, "text/html", "<p><a href=\"apri\">Apri</a></p><p><a href=\"stop\">Stop</a></p><p><a href=\"chiu\">Chiudi</a></p>");
  }

  if (server.uri().startsWith("/data/get/rob/")) {
    String s = server.uri().substring(14);
    server.send(200, "text/html", "<p><b>" + s + "</b><br>" + abbina_variabile(s) + "</p>" + lista_variabili_disponibili());

  }

  if (server.uri().startsWith("/data/get/")) {
    String s = server.uri().substring(10);
    //Serial.print(F("sarr "));Serial.println(s);
    if (s == "segno") {
      server.send(200, "text/plain", segno);
    }
    if (s == "barr") {
      BARRA = String(millis());
      server.send(200, "text/plain", BARRA);
    }
    if (s == "puls") {
      server.send(200, "text/html", "<p>" + String(_Pulsanti) + "</p>");
      //        server.send(200, "text/plain", _Pulsanti);
    }

    if (s == "pos") {
      String posrg = "";
      for (int i = 1; i <= Numero_Ante_Slave; i++) {
        posrg = posrg + " - " + String(Pos_Anta[i]);
      }
      server.send(200, "text/plain", posrg );
    }
    if (s == "addr") {

      Serial.println("ante"); Serial.println(Numero_Ante_Slave);
      addrg = "";
      for (int i = 1; i <= Numero_Ante_Slave; i++) {
        addrg = addrg + " - " + String(addr[i]);
      }
      server.send(200, "text/plain", addrg );
      server.send(200, "text/html", "<p>" + addrg + "</p>");
    }
    if (s == "vars") {
      vars = allDataFiller();
      server.send(200, "text/plain", vars);
    }
    if (s == "error") {
      server.send(200, "text/plain", error);
    }
    s.remove(0);
  }


  if (!handleFileRead(server.uri()))                  // send it if it exists
    server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)

  path = path.substring(1);
  //Serial.print("path ");
  if (path.endsWith("/")) path += "indexweb.htm";          // If a folder is requested, send the index file
  String contentType = getContentType(path);
  //Serial.println("handleFileRead: " + path);
  //Serial.println("getContentType: " + contentType);// Get the MIME type
  String out = "";
  char tmp[BUFFER_SIZE - 1];
  int cont = 0;
  if (SD.exists(path)) { // If the file exists, either as a compressed archive, or normal
    //Serial.print("trov ");
    File file = SD.open(path, FILE_READ);
    // size_t sent = server.streamFile(file, contentType);    // Send it to the client
    //unsigned int b = file.size();
    while (file.available()) {
      // String line = file.readStringUntil('\n');
      char c = file.read();
      //out=out+line+'\n';
      tmp[cont] = c;
      cont++;
      //Serial.println(line);
      //line.remove(0);
    }
    file.close();                                          // Close the file again
    // Serial.println(tmp);

    server.send(200, contentType , tmp);
    memset(tmp, 0, BUFFER_SIZE);
    //Serial.print("Dopo ");Serial.println(tmp);
    out.remove(0);

    return true;
  } else {
    Serial.println("non trovato: " + path);
  }
  //Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

void handleFileUpload() { // upload a new file to the
  Serial.println("upload  ");
  HTTPUpload& upload = server.upload();
  //Serial.print("upload  "); Serial.println(UPLOAD_FILE_START);
  //Serial.print("upload.filename  "); Serial.println(upload.filename);
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    //Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = SD.open(filename, FILE_WRITE | O_TRUNC);            // Open the file for writing in SD(create if it doesn't exist)
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {                                   // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      //Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Location", "/success.htm");     // Redirect the client to the success page
      server.send(303);
    } else {
      //Serial.println("File upload failed");
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
  //set_reset_ridotto();
}
void printWiFiStatus()
{
  //print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  //  Serial.print("To see this page in action, open a browser to http://");
  // Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void handleRoot()
{


  digitalWrite(led, 1);
  char temp[BUFFER_SIZE];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;

  hr = hr % 24;

  snprintf(temp, BUFFER_SIZE - 1,
           "<html>\
<head>\
<meta http-equiv='refresh' content='5'/>\
<title>%s</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h1>Hello from %s</h1>\
<h3>running WiFiWebServer</h3>\
<h3>on %s</h3>\
<p>Uptime: %d d %02d:%02d:%02d</p>\
<img src=\"/test.svg\" />\
</body>\
</html>", BOARD_NAME, BOARD_NAME, SHIELD_TYPE, day, hr, min % 60, sec % 60);

  server.send(200, F("text/html"), temp);
  // digitalWrite(led, 0);
}

void handleNotFound()
{
  myhandlenotfound();
  /*  digitalWrite(led, 1);

    String message = F("File Not Found\n\n");

    message += F("URI: ");
    message += server.uri();
    message += F("\nMethod: ");
    message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
    message += F("\nArguments: ");
    message += server.args();
    message += F("\n");

    for (uint8_t i = 0; i < server.args(); i++)
    {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, F("text/plain"), message);

    digitalWrite(led, 0);*/
}

String out;

void drawGraph()
{
  String out;
  out.reserve(3000);
  char temp[70];

  out += F("<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"310\" height=\"150\">\n");
  out += F("<rect width=\"310\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"3\" stroke=\"rgb(0, 0, 0)\" />\n");
  out += F("<g stroke=\"blue\">\n");
  int y = rand() % 130;

  for (int x = 10; x < 300; x += 10)
  {
    int y2 = ( rand() + millis() ) % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"2\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += F("</g>\n</svg>\n");

  server.send(200, F("image/svg+xml"), out);
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
  if (trova == "vars") {
    vars = allDataFiller();
    return vars;
  }
  if (trova == "error") {
    return error;
  }
  if (trova == "") {
    return "<b>Scegli cosa vedere</b>";
  }
  return "<b>non trovato</b>";
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

String lista_variabili_disponibili() {
  String da_scrivere = "";
  da_scrivere = "<p>";
  da_scrivere = da_scrivere + "<a href=\"imp\">imp</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"My_Buffer\">My_Buffer</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"cicli_test\">cicli_test</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"_Pulsanti\">_Pulsanti</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"addr_scanner\">addr_scanner</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"vars\">vars</a><b> -- </b>";
  da_scrivere = da_scrivere + "<a href=\"set\">set</a><b> -- </b>";
  da_scrivere = da_scrivere + "</p>";
  return da_scrivere;
}
