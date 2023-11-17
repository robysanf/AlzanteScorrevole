/*void socket(){
  check_status();

  
  WebsocketsClient client = sserver.accept();
 
  if (client.available())
  {
    WebsocketsMessage msg = client.readNonBlocking();

    // log
    Serial.print("Got Message: ");
    Serial.println(msg.data());
    String st=String(conta)+"£"+Stato_Anta[1]+" - "+Stato_Anta[2];
    
    // return echo
    client.send("Echo1: " + st);
   // client.send("Echo2: " + msg.data());
   // client.send("Echo3: " + msg.data());
   // client.send("Echo4: " + msg.data());
    // close the connection
    client.close();
  }
}
  
void heartBeatPrint(void)
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("H");        // H means server WiFi connected
  else  
    Serial.print("F");        // F means server WiFi not connected
    
  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(" ");
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

  //KH
#define HEARTBEAT_INTERVAL    1000L
  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}*/
