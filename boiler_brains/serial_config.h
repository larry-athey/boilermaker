//------------------------------------------------------------------------------------------------
// Boilermaker SCR/SSR Brain | (CopyLeft) 2024-Present | Larry Athey (https://panhandleponics.com)
//------------------------------------------------------------------------------------------------
void displayMenu() {
  Serial.write("\033[2J\033[H\n\n");
  Serial.println("Boilermaker Configuration Menu:");
  Serial.println("1. Set SSID");
  Serial.println("2. Set Password");
  Serial.println("3. Set DHCP or Static IP");
  Serial.println("4. Connect to WiFi");
  Serial.println("5. Set Slave 1 IP");
  Serial.println("6. Set Slave 2 IP");
  Serial.println("7. Set Slave 3 IP");
  Serial.println("8. Set Slave 4 IP");
  Serial.print("Choose an option: ");
}
//------------------------------------------------------------------------------------------------
