//------------------------------------------------------------------------------------------------
// Boilermaker SCR/SSR Brain | (CopyLeft) 2024-Present | Larry Athey (https://panhandleponics.com)
//
// Inline functions used for modular unit organization
//------------------------------------------------------------------------------------------------
inline String formatMillis(unsigned long millisValue) { // Converts a timestamp to a HH:MM:SS formated time
  unsigned long seconds = millisValue / 1000; // Convert milliseconds to seconds
  unsigned long minutes = seconds / 60;       // Convert seconds to minutes
  unsigned long hours = minutes / 60;         // Convert minutes to hours

  // Calculate the remaining seconds, minutes after accounting for hours and minutes
  seconds %= 60;
  minutes %= 60;

  char buffer[10]; // Enough space for "HH:MM:SS" + null terminator
  snprintf(buffer,sizeof(buffer),"%02ld:%02ld:%02ld",hours,minutes,seconds);

  return String(buffer);
}
//------------------------------------------------------------------------------------------------
inline byte SlaveTotal() { // Returns the total number of configured slave units
  byte Total = 0;
  if (slaveIP1 != "") Total ++;
  if (slaveIP2 != "") Total ++;
  if (slaveIP3 != "") Total ++;
  if (slaveIP4 != "") Total ++;
  return Total;
}
//------------------------------------------------------------------------------------------------
inline String GetSlaveIP(byte WhichOne) { // Returns the IP address of a slave (1..4) as a String type
  if (WhichOne == 1) return slaveIP1;
  if (WhichOne == 2) return slaveIP2;
  if (WhichOne == 3) return slaveIP3;
  if (WhichOne == 4) return slaveIP4;
}
//------------------------------------------------------------------------------------------------
inline bool PingSlave(byte WhichOne) { // Returns true if a slave unit (1..4) is alive and pinging
  if (GetSlaveIP(WhichOne) == "") return false;
  bool PingTest = Ping.ping(GetSlaveIP(WhichOne).c_str(),2);
  return PingTest;
}
//------------------------------------------------------------------------------------------------
inline byte PingAllSlaves() { // Pings all slaves and returns the number of how many are pinging
  byte Total = 0;
  if (SlaveTotal() == 0) return Total;
  if (Ping.ping(GetSlaveIP(1).c_str(),2)) Total ++;
  if (Ping.ping(GetSlaveIP(2).c_str(),2)) Total ++;
  if (Ping.ping(GetSlaveIP(3).c_str(),2)) Total ++;
  if (Ping.ping(GetSlaveIP(4).c_str(),2)) Total ++;
  return Total;
}
//------------------------------------------------------------------------------------------------
inline bool UpdateSlave(byte WhichOne,String Query) { // Update a slave unit with an HTTP API call
  HTTPClient http;

  http.begin("http://" + GetSlaveIP(WhichOne) + Query);
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
inline void UpdateAllSlaves(String Query) { // Updates all slaves with the same HTTP API call
  if (SlaveTotal() > 0) {
    for (byte x = 1; x <= 4; x ++) {
      if (PingSlave(x)) UpdateSlave(x,Query);
    }
  }
}
//------------------------------------------------------------------------------------------------
