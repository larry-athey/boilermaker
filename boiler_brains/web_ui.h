//------------------------------------------------------------------------------------------------
// Boilermaker SCR/SSR Brain | (CopyLeft) 2024-Present | Larry Athey (https://panhandleponics.com)
//------------------------------------------------------------------------------------------------
void PingSlaves() {

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
