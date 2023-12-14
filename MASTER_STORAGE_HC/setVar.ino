void setVar() {
  /* tabella parametri sullo slave
    3331: // -- PER CAMBIARE LA VELOCITA' MASSIMA
    3332: // -- PER CAMBIARE LA VELOCITA' MINIMA
    3333: // -- PER CAMBIARE LA VELOCITA' IN COLLISIONE  OPEEN CLOSE FAAST SETT1
    3334: // -- PER CAMBIARE LA DIREZIONE DEL MOTORE
    3336: // -- PER CAMBIARE IL VALORE DI ATTRITO
    3337: // -- E' LIBERO
    3338: // -- PER CAMBIARE IL VALORE DI INERZIA
    3339: // -- PER CAMBIARE IL VALORE DI PESO
    3310: // -- PER SETTARE IL VALORE DI POS_APERTO
  */
  inviaTutti("3311", String(imp * 100));
  delay(10);
  Serial.print("\n velocita_crocera_MAX da storage= " + String(velocita_crocera_MAX));
  Serial.print("\n velocita_crocera_LENTA da storage = " + String(velocita_crocera_LENTA));
  Serial.print("\n velocita_crocera_COL cablata = " + String(velocita_crocera_COL));
  delay(10);
  inviaTutti("3333", String(velocita_crocera_COL * 100));
  delay(10);
  inviaOgnunoString("3336", attrito);
  delay(10);
  inviaOgnunoString("3334", motore);
  delay(10);
  inviaOgnunoString("3335", encoder);
  delay(10);
  inviaOgnunoInt("3338", inerzia);
  delay(10);
  inviaOgnunoInt("3339", peso);
  delay(10);
  inviaOgnunoLong("3310", pos_aperto);
  Per_Centuale = (pos_aperto[1] - (Distanza * ((Numero_Ante_Slave + 1) - 1))) / 100;
  delay(10);
  calcola_velocita();
}
