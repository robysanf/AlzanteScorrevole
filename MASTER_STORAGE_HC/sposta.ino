void sposta(String anta_code){
  
  int ipCode=anta_code.substring(0,2).toInt();
  int p1=anta_code.indexOf(',');
  String sr1 = anta_code.substring(p1+1);

  int p2=sr1.indexOf(',');
  String enc=sr1.substring(0,p2);
  String mot = sr1.substring(p2+1);
  Serial.print("sr1");Serial.println(sr1);
  Serial.print("enc");Serial.println(enc);
  Serial.print("mot");Serial.println(mot);
  int anta_num=0;
  for(int i=1;i<=Numero_Ante_Slave;i++){
      if(ipCode == Ante_Slave[i]){
        anta_num=i;
        }
    }
    Serial.println("anta_num");Serial.println(anta_num);
  long old_pos=Pos_Anta[anta_num];
  inVia(anta_num,"3335"+enc,5);
  delay(100);
  ricEvi();
  inVia(anta_num,"3334"+mot,4);
  delay(100);
  ricEvi();
  inVia(anta_num,"2223",3);
  delay(1500);
  ricEvi();
  inVia(anta_num,"2221",2);
  ricEvi();
  delay(1000);
  ricEvi();
  long delta_pos=Pos_Anta[anta_num]-old_pos;
  segno=anta_code+";"+String(delta_pos);
  
}
/*
void spostaold(String i2c_code){
  Serial.print("\n i2c_code = "); Serial.print(i2c_code);
 int i = i2c_code.substring(0,2).toInt();;
 long prima = 0;
 long dopo = 0;
 String segno_encoder = "-";
 String invio = "";
 char _invio[32] ="";
 
 String tmprima=  Ricevi_SLAVE(i); 
 int p=tmprima.indexOf('#');
 prima = tmprima.substring(1,p).toInt();  // -- MI SALVO IL VALORE DI POS prima del move
 Serial.print("\n prima "+String(prima));
 if ( i2c_code.substring(2,3) == "+" ){invio = "33341/";} else {invio = "3334-1/";}
 invio.toCharArray(_invio, 32);
 ordina(_invio, i, 18);                               // -- passo il valore di motore
 delay(90);
 if ( i2c_code.substring(3,4) == "+" ){invio = "33351/";} else {invio = "3335-1/";}
 invio.toCharArray(_invio, 32);
 ordina(_invio, i, 18);                                   // -- passo il valore di encoder
 delay(90);
 ordina("3330/", i, 18);                               // -- ordino di aprire
 delay(2000);
 String tmdopo=  Ricevi_SLAVE(i); 
 p=tmdopo.indexOf('#');// -- aspetto
 dopo = tmdopo.substring(1,p).toInt();                 // -- MI SALVO IL VALORE DI POS dopo il move
  Serial.print("\n dopo "+String(dopo));
 ordina("2221/", i, 18);                                 // -- ordino di fermare
 if (dopo > prima ){segno_encoder = "+";}               // -- faccio il confronto per sapere se il pos aumentato o diminuito
 invio = "enco";
 invio = invio + i + segno_encoder + "/";               // -- costruisco il comando con i dati
 Serial.print("\n invio = "); Serial.print(invio);
 invio.toCharArray(_invio, 32);                         // -- lo trasformo in char
 ordina_Web(_invio);                                 // -- mando allo yun il valore + 0 - a seconda della lettura dell'encoder
  
}*/



 
