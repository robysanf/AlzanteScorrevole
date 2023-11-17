 boolean limit() {
  int amp_2 = 0;
  int x1, x2, y1, y2;
  if (tensione < 0) {
    tensione = -tensione;
  }
  if (tensione == 0) {
    tensione = 1;
  }
  if (tensione < 50) {
    x1 = 0;
  }
  else {
    x1 = floor(tensione / 50.00);
    if(tensione>=1150)
      x1=23;
      
  }
  x2 = (x1 + 1) * 50;
  y1 = limiti[x1];
  y2 = limiti[x1 + 1];
  amp_2 = y1 + (tensione - (x1 * 50)) * (y2 - y1) / (x2 - x1 * 50);
  int amp_1 = fai_media();
  Serial.print("amp 1 = "); Serial.println(amp_1);
  
  amp_2 = ((amp_2 + (((limiti[24]) - amp_2) * marginelimite / 100))+(amp_2+(amp_2*marginelimite / 100)))/2;
  Serial.print("amp 2 = "); Serial.println(amp_2);
  
  if (amp_1 >= amp_2) {
    Serial.print("superato limit = "); Serial.print(amp_1);
    Serial.print("-AMP_2 = "); Serial.print(((amp_2 + (((limiti[24]) - amp_2) * marginelimite / 100))+(amp_2+(amp_2*marginelimite / 100)))/2);
    Serial.print(" - amp_2 curva = "); Serial.print(amp_2 + ((limiti[24] - amp_2) * marginelimite / 100));
    Serial.print(" - tensione = "); Serial.println(tensione);
    return true;
  }
  return false;
}


void setLimitCurve() {

  //spinge per x millisecodi mantenendo la tensione fissa e memorizzando la corrente massima
  str_limit = "";
  for ( int i = 100; i <= TopSpeed; i = i + 100) {

    int consumo = 0;
    int consumo_max = 0;
    for (int c = 10; c <= i; c = c + 10) {
      md.setM2Speed(c * _Dir * motore);
      consumo = md.getM2CurrentMilliamps();
      if (consumo > 3000) {
        md.setM2Speed(0);
        consumo = 3000;
      }
      consumo_max = max(consumo, consumo_max);
      delay(100);
    }
    md.setM2Speed(0);
    str_limit = str_limit + consumo_max + "--";
    limiti[i / 100] = consumo_max;
  }//for
  str_limit = str_limit.substring(0, str_limit.length() - 1) ; //verificare il length-1
}


void spacchetta_limit(String str) {
  char list[255];
  str.toCharArray(list, 255);
  char * pch;
  pch = strtok (list, "--");
  int lim = String(pch).toInt();
  limiti[1] = lim;
  lim = 0;
  for (int i = 2; i < 13; i++) {
    lim = 0;
    pch = strtok (NULL, "--");
    lim = String(pch).toInt();
    limiti[i] = lim;
  }
}

int fai_media() {
  conta_corrente = conta_corrente + 1;
  if (conta_corrente > 19) {
    conta_corrente = 0;
  }
  corrente[conta_corrente] = md.getM2CurrentMilliamps();
  int tot = 0;
  for (int i = 0; i < 20; i++) {
    tot = tot + corrente[i];
  }
  return tot / 20;
}
