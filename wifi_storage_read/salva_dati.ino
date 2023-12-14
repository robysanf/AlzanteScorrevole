void salva_dati() {

String message = "";

  for (int i = 0; i < server.args(); i++) {


    message += server.argName(i) + "=";            //Get the name of the parameter
    message += server.arg(i) + "#";              //Get the value of the parameter

  }

  Storage_SaveAll(message);

}
