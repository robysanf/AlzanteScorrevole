String dataFiller(String toFind) {
  String pathini = "/data.ini";

  if (SD.exists(pathini)) { // If the file exists, either as a compressed archive, or normal

    File file = SD.open(pathini, FILE_READ);                    // Open the file
    while (file.available()) {
      //Lets read line by line from the file
      String line = file.readStringUntil('\n');
      //writelog(line);
      int p = line.indexOf("=");
      String var = line.substring(0, p);
      String value = line.substring(p + 1);
      if (toFind == var) {
        file.close();
        //writelog("trovata "+value);
        return value;
      }
    }
    file.close();                                          // Close the file again
    //Serial.println(String("\tSent file: ") + path);
    return "";
  }
}

String allDataFiller() { //********costruisce il JSON
  String result = "{";
  String path = "/data.ini";

  if ( SD.exists(path)) { // If the file exists, either as a compressed archive, or normal
    File file = SD.open(path, FILE_READ);                    // Open the file
    while (file.available()) {
      //Lets read line by line from the file
      String line = file.readStringUntil('\n');
      //writelog(line);
      int p = line.indexOf("=");
      String var = line.substring(0, p);
      if (var != "") {
        String value = line.substring(p + 1);
        value = value.substring(0, value.length() - 1);
        value.replace("/", "");
        result = result + "\"" + var + "\":\"" + value + "\",";
        //writelog(result);
      }

    }
    file.close();
    result = result.substring(0, result.length() - 1) + "}";
    //writelog("ALLDATAFILLER " + result);
    //Serial.println(String("\tSent file: ") + path);
    return result;
  }
}

String dataRead(String toFind) {
  String path = "data.ini";
  File file = SD.open(path, FILE_READ);
  String line = "";
  if (file) {
    // @-- Serial.println("data.ini:");
    // @-- read from the file until there's nothing else in it:
    while (file.available()) {
      line = file.readStringUntil('\r');
      // @-- Serial.println(line); //Printing for debugging purpose
      int p = line.indexOf("=");
      String var = line.substring(1, p);
      // @-- Serial.println("var "+var);
      // @-- Serial.println("tofind "+toFind);
      String value = line.substring(p + 1);
      if (toFind == var) {
        file.close();
        // @-- Serial.println("trovata " + value);
        return value;
      }
    }
    // close the file:
    file.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening data.ini");
  }
}

boolean dataSave(String key, String value) {
  String path = "/data.ini";
  String pathw = "/data.tmp";
  Serial.println("copy");
  //writelog("datasave " + path);
  if (SD.exists(path)) {
    Serial.println("saveexist");
    File filew = SD.open(pathw, FILE_WRITE);
    File file = SD.open(path, FILE_READ);                    // Open the file
    //size_t sent = server.streamFile(file, contentType);    // Send it to the client
    while (file.available()) {
      //Lets read line by line from the file
      String line = file.readStringUntil('\n');
      line = line.substring(0, line.length() - 1);
      if (line.startsWith(key)) {
        String s = key + "=" + value + "/";
        filew.println(s);
        // writelog("datasave3 "+s);
      } else {
        filew.println(line);
      }
      //writelog("datasave2 "+line+"*");
    }

    file.close();
    filew.close();// Close the file again
    //pathw contiene i dati modificati adesso posso riscriverli nel file di partenza
    if (SD.exists(pathw))
    {
      SD.remove(path);
      copyfile(path, pathw);
    }

    return true;
  }
  return false;

}

boolean dataSaveLimit(int num_anta, String value) {
  String path = "/limit.ini";
  String pathw = "/limit.tmp";
  Serial.println("copy");
  String key = "" + num_anta;
  //writelog("datasave " + path);
  if (SD.exists(path)) {
    Serial.println("saveexist");
    File filew = SD.open(pathw, FILE_WRITE);
    File file = SD.open(path, FILE_READ);                    // Open the file
    //size_t sent = server.streamFile(file, contentType);    // Send it to the client
    while (file.available()) {
      //Lets read line by line from the file
      String line = file.readStringUntil('\n');
      line = line.substring(0, line.length() - 1);
      if (line.startsWith(key)) {
        String s = key + "=" + value + "/";
        filew.println(s);
        // writelog("datasave3 "+s);
      } else {
        filew.println(line);
      }
      //writelog("datasave2 "+line+"*");
    }

    file.close();
    filew.close();// Close the file again
    //pathw contiene i dati modificati adesso posso riscriverli nel file di partenza
    if (SD.exists(pathw))
    {
      SD.remove(path);
      copyfile(path, pathw);
    }

    return true;
  }
  return false;

}



boolean copyfile(String pathfinale, String pathtemp) {                    //usato per rinominare il file
  Serial.println("copy");                                                 // il file pathtemp viene eliminato e pathfinale sovrascritto se eiste
  //writelog("path finale " + pathfinale + "pathtemp " + pathtemp);
  if (SD.exists(pathtemp)) {
    File filew = SD.open(pathfinale, FILE_WRITE | O_TRUNC);
    File file = SD.open(pathtemp, FILE_READ);                             // Open the file
    while (file.available()) {
      //Lets read line by line from the file
      String line = file.readStringUntil('\n');
      line = line.substring(0, line.length() - 1);
      filew.println(line);
      //writelog("copy " + line);
    }
    file.close();
    filew.close();       // Close the file again
                         //pathw contiene i dati modificati adesso posso riscriverli nel file di partenza
    SD.remove(pathtemp);
    return true;
  }
  return false;
}

void writelog(String tolog) {
  int conta49=0;  
  conta49=read49();
  String pathlog_now="/"+String(conta49)+pathlog;
  //Serial.print("pathlog_now ");Serial.println(pathlog_now);
  File filesize = SD.open(pathlog_now);              // Open the file
  unsigned long t=filesize.size();
  filesize.close();
  Serial.print("File Size ");Serial.println(t); 
  if(t>1000){ 
    conta49++; 
    if(conta49>30){
       conta49=0;
      }
      File file49 = SD.open("49.txt", FILE_WRITE | O_TRUNC); 
      file49.print(conta49); 
      file49.close();  
      pathlog_now="/"+String(conta49)+pathlog;
      
      File file = SD.open(pathlog_now, FILE_WRITE | O_TRUNC); // Overwrite the file
      file.print("<br>");
      file.close();
    }
  Serial.print("pathlog_now ");Serial.println(pathlog_now);   
  File file = SD.open(pathlog_now, FILE_WRITE);              
  Serial.println(file.size()); 
  file.print(tolog + "<br>");
  file.close();
  //clear_old_log(conta49); 
}

int read49(){
 //legge il file che memorizza la quantitÃ  di file cui siamo arrivati con i log
   File file49 = SD.open("49.txt", FILE_READ);
   //sono al massimo 9 giorni quindi leggo solo un char cosi è piu veloce
   char list[1];
   file49.read(list, 1);
  
   file49.close();
  
   return String(list).toInt();
  
  }

void writelogStatus() {
  String s=String(millis());
   for (int i = 1; i<= Numero_Ante_Slave; i = i +1){
    s=s+"<BR>Stato Anta "+Stato_Anta[i]+"<BR>";
    s=s+"Stato Alzata "+Stato_Alzata[i]+"<BR>";
    s=s+"Tensione Batteria "+Batteria_Tensione[i]+"<BR>";
  }
  //Serial.println("memoria ");Serial.println(freeRam());
  s=s+"Memoria Libera "+freeRam()+"<BR>";
  writelog(s); 
}

void clear_old_log(int value49){
  if(Stato_Alzata[1]!="D")
  {
    return;
    }
  // ATTENZIONE IL WIRITELOG DEVE CREARE FILE DI 1K-2K MASSIMO
  
  if(value49 >= step_del_log && SD.exists("/"+String(value49-step_del_log)+pathlog))
  {  
       SD.remove("/"+String(value49-step_del_log)+pathlog);
     } 
}


void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
