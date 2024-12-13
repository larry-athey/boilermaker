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
String GetSlaveIP(byte WhichOne) {
  if (WhichOne == 1) return slaveIP1;
  if (WhichOne == 2) return slaveIP2;
  if (WhichOne == 3) return slaveIP3;
  if (WhichOne == 4) return slaveIP4;
}
//------------------------------------------------------------------------------------------------
void PingSlave(byte WhichOne) {

}
//------------------------------------------------------------------------------------------------
void PingAllSlaves() {

}
//------------------------------------------------------------------------------------------------
void UpdateSlave(byte WhichOne,String Query) {
  HTTPClient http;
  http.begin("http://example.com/json-endpoint");  // Replace with your JSON endpoint

  // Send HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println(payload);  // This will print the JSON string
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();  // Close connection
}
//------------------------------------------------------------------------------------------------
void UpdateAllSlaves(String Query) {
  PingAllSlaves();
}
//------------------------------------------------------------------------------------------------
void SynchronizeSlave(byte WhichOne) {

}
//------------------------------------------------------------------------------------------------
void SynchronizeAllSlaves() {
  PingAllSlaves();
}
//------------------------------------------------------------------------------------------------
