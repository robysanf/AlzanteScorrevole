void  inVia(int client, String msg, int chi) {

  //Serial.println("mi ha chiamato :" + String(chi));
  int num_ip = Ante_Slave[client];
//  String ip_addr = ip_prefix + String(num_ip);
//  String port_addr = port_prefix + String(num_ip);
  if (msg.substring(0, 4) == "0000") {
    if (Risposta_Comando[client] !=  Comando[client] ) {
      msg = Comando[client];
    }
    else {
      Comando[client] = msg;
    }
  }
  if (msg.substring(0, 4) != "0000") {
    Comando[client] = msg;
  }
  conta++;
  char list[255];
      msg =  String(Ante_Slave[client]) + "," + msg + "," +conta+"@";
      
      msg.toCharArray(list, 255);
  Serial.print("Invio : "); Serial.print(Ante_Slave[client]); Serial.print("  -  "); Serial.println(msg);
  Serial1.write(list);
  tempo_invia = micros();

}

//manda lo stesso valore a tutti gli slave
void inviaTutti(String commandToSlave, String parametro) {
  for (int i = 1; i <= Numero_Ante_Slave; i = i + 1) {
    Serial.print("Invia tutti : "); Serial.print(i);
    inVia(i, commandToSlave + parametro, 16);
    attendi_risposta(i, 10, commandToSlave);
    delay(10);

    // inVia(i,"0000",15); lo invia attendi_risposta
  }
}
//manda a ogni slave il suo valore intero
void inviaOgnunoInt(String commandToSlave, int parametro[]) {
  for (int i = 1; i <= Numero_Ante_Slave; i = i + 1) {
    inVia(i, commandToSlave + parametro[i], 14);
    attendi_risposta(i, 10, commandToSlave);
    delay(10);
    // inVia(i,"0000",13);lo invia attendi_risposta
  }
}

//manda a ogni slave il suo valore String
void inviaOgnunoString(String commandToSlave, String parametro[]) {
  for (int i = 1; i <= Numero_Ante_Slave; i = i + 1) {
    inVia(i, commandToSlave + parametro[i], 12);
    attendi_risposta(i, 10, commandToSlave);
    delay(5);
    //inVia(i,"0000",1111); lo invia attendi_risposta
  }
}
//manda a ogni slave il suo valore long
void inviaOgnunoLong(String commandToSlave, long parametro[]) {
  for (int i = 1; i <= Numero_Ante_Slave; i = i + 1) {
    inVia(i, commandToSlave + parametro[i], 11);
    attendi_risposta(i, 10, commandToSlave);
    delay(10);
    //inVia(i,"0000",10); lo invia attendi_risposta
  }
}

void attendi_risposta(int anta, int tentativi, String comando) {
  for (int c = 0; c < tentativi ; c++) { //provo num. "tentativi" volte per essere sicuro che lo ricevo
    Serial.print("attendi risposta a : "); Serial.println(comando);
    delay(250);
    ricEvi();
    Serial.print("attendi risposta comando_anta : "); Serial.println(Risposta_Comando[anta]);
    if (Risposta_Comando[anta].substring(0, 4) == comando) {
      break;
    }
    inVia(anta, "0000", 9);
  }
}
