//------------------------------------------------------------------------------------------------
// Boilermaker SCR/SSR Brain | (CopyLeft) 2024-Present | Larry Athey (https://panhandleponics.com)
//------------------------------------------------------------------------------------------------
byte SlaveTotal() { // Returns the total number of configured slave units
  byte Total = 0;
  if (slaveIP1 != "") Total ++;
  if (slaveIP2 != "") Total ++;
  if (slaveIP3 != "") Total ++;
  if (slaveIP4 != "") Total ++;
  return Total;
}
//------------------------------------------------------------------------------------------------
String GetSlaveIP(byte WhichOne) { // Returns the IP address of a slave (1..4) as a String type
  if (WhichOne == 1) return slaveIP1;
  if (WhichOne == 2) return slaveIP2;
  if (WhichOne == 3) return slaveIP3;
  if (WhichOne == 4) return slaveIP4;
}
//------------------------------------------------------------------------------------------------
bool PingSlave(byte WhichOne) { // Returns true if a slave unit (1..4) is alive and pinging
  bool PingTest = Ping.ping(GetSlaveIP(WhichOne).c_str(),3);
  return PingTest;
}
//------------------------------------------------------------------------------------------------
byte PingAllSlaves() { // Pings all slaves and returns the number of how many are pinging
  byte Total = 0;
  if (SlaveTotal() == 0) return Total;
  if (Ping.ping(GetSlaveIP(1).c_str(),3)) Total ++;
  if (Ping.ping(GetSlaveIP(2).c_str(),3)) Total ++;
  if (Ping.ping(GetSlaveIP(3).c_str(),3)) Total ++;
  if (Ping.ping(GetSlaveIP(4).c_str(),3)) Total ++;
  return Total;
}
//------------------------------------------------------------------------------------------------
bool UpdateSlave(byte WhichOne,String Query) { // Update a slave unit with an HTTP API call
  HTTPClient http;

  http.begin("http://" + GetSlaveIP(byte WhichOne) + Query);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    //Serial.println("HTTP Response code: " + String(httpResponseCode));
    //Serial.println(payload);
    http.end();
    return true;
  } else {
    //Serial.print("Error code: " + String(httpResponseCode));
    http.end();
    return false;
  }
}
//------------------------------------------------------------------------------------------------
void UpdateAllSlaves(String Query) { // Updates all slaves with the same HTTP API call
  if (SlaveTotal() > 0) {
    for (byte x = 1; x <= 4; x ++) {
      if (PingSlave(x)) UpdateSlave(x,Query);
    }
  }
}
//------------------------------------------------------------------------------------------------
