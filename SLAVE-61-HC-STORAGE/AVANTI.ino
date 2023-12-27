 void avanti_2()  {
  if (giro == 4) {
    giro = 0;
    T1 = micros();                                      // -- fermo il tempo
    T3 = T1 - T2;                                       // -- calcolo il tempo trascorso
    T2 = T1;
    V_M = (((1 / (imp/8)) * 100000) / T3) * 10;
  }
giro ++;
}
void avanti_3()  {
  if (digitalRead(3) == digitalRead(2)) {
    pos = pos - encoder ;
  }    // -- decremento il pos
  else {
    pos = pos + encoder ;
  }        // -- incremento il pos

}
