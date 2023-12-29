void ascolta_sock() {

  client = new WebsocketsClient;

  if (client)
  {
    *client = SocketsServer.accept();

    if (client->available())
    {
      WebsocketsMessage msg = client->readBlocking();

      // log
      Serial.print("Got Message: ");
      Serial.println(msg.data());
      strwebHost = String(msg.data());
      Serial.print("strwebhost "); Serial.println(strwebHost);
      strweb = strwebHost.substring(0, 4);
      strweb2 = strwebHost.substring(4);
      if ( strweb == "save") {
        Salva_Socket(strweb2);
      }
  
      String risposta = Storage_Read_WebSock();
      // for ( int i = 1; i <= Numero_Ante_Slave  ; i = i + 1) {
     
      
      client->send("Echo: " + risposta);

      Serial.print("oooooooooooo "); Serial.println(strwebHost);
      // close the connection
      client->close();
    }
    delete client;
  }
}

void Salva_Socket(String varList) {
  char list[255];
  varList.toCharArray(list, 255);
  char * pch;
  pch = strtok (list, "#");
  if (pch == NULL) {
    return;
  }
  String s = String(pch);
  String nome_variabile = s.substring(0, s.indexOf("="));
  String valore = s.substring(s.indexOf("=") + 1);
  Serial.print("leggo"); Serial.print(nome_variabile); Serial.print(" = "); Serial.println(valore);

  while (pch != NULL)
  {
    dataSave(nome_variabile, valore);
    pch = strtok (NULL, "#");
    if (pch == NULL) {
      break;
    }
    s = String(pch);
    nome_variabile = s.substring(0, s.indexOf("="));
    valore = s.substring(s.indexOf("=") + 1);
  }
}

String Storage_Read_WebSock() {
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
  return stampa;
}
