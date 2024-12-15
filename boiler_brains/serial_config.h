//------------------------------------------------------------------------------------------------
// Boilermaker SCR/SSR Brain | (CopyLeft) 2024-Present | Larry Athey (https://panhandleponics.com)
//------------------------------------------------------------------------------------------------
void ShowConfig() { // Send an ANSI clear screen sequence and display all configuration settings
  Serial.write("\033[2J\033[H\n\n");
  Serial.println("DeviceName: " + DeviceName);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("wifiConnected: Yes");
  } else {
    Serial.println("wifiConnected: No");
  }
  if (wifiMode == 0) {
    Serial.println("wifiMode: DHCP");
  } else {
    Serial.println("wifiMode: Static IP");
  }
  Serial.println("wifiSSID: " + wifiSSID);
  Serial.println("wifiPassword: " + wifiPassword);
  Serial.println("wifiIP: " + wifiIP);
  Serial.println("wifiMask: " + wifiMask);
  Serial.println("wifiGateway: " + wifiGateway);
  Serial.println("wifiDNS: " + wifiDNS);
  Serial.println("slaveIP1: " + slaveIP1);
  Serial.println("slaveIP2: " + slaveIP2);
  Serial.println("slaveIP3: " + slaveIP3);
  Serial.println("slaveIP4: " + slaveIP4);
  Serial.println("CorrectionFactor: " + String(CorrectionFactor));
  Serial.println("");
}
//------------------------------------------------------------------------------------------------
void ConfigMenu() { // Display the configuration menu
  Serial.println("Boilermaker Configuration Menu:");
  Serial.println("  0. Set Device Name");
  Serial.println("  1. Set SSID");
  Serial.println("  2. Set Password");
  Serial.println("  3. Set DHCP or Static IP");
  Serial.println("  4. Connect to WiFi");
  Serial.println("  5. Set Slave 1 IP");
  Serial.println("  6. Set Slave 2 IP");
  Serial.println("  7. Set Slave 3 IP");
  Serial.println("  8. Set Slave 4 IP");
  Serial.println("  9. Set Correction Factor");
  Serial.print("Choose an option: ");
}
//------------------------------------------------------------------------------------------------
void PurgeBuffer() { // Purge anything waiting in the receive buffer
  while (Serial.available()) Serial.read();
}
//------------------------------------------------------------------------------------------------
String ReadInput() { // Read from the serial port until the user presses the ENTER key
  String input = "";
  char c;
  // Teminal program should have ANSI emulation enabled, VT100 may not handle backspaces correctly
  while (true) {
    if (Serial.available() > 0) {
      c = Serial.read(); // Read the incoming character
      // Echo the character back to the serial monitor
      Serial.print(c);
      if (c == '\r' || c == '\n') { // Check if it's a newline
        return input; // Return the collected input when Enter is pressed
      } else if (c == '\b' && input.length() > 0) { // Handle backspace
        // Remove the last character from input
        input.remove(input.length() - 1);
        // Clear the last character from the display
        Serial.print("\b \b"); // Backspace, space, backspace
      } else {
        // Append the character to the input string
        input += c;
      }
    }
  }
}
//------------------------------------------------------------------------------------------------
void get_wifiSSID() { // Get the WiFi SSID (network name)
  Serial.print("Enter WiFi SSID: ");
  wifiSSID = ReadInput();
}
//------------------------------------------------------------------------------------------------
void get_wifiPassword() { // Get the WiFi password
  Serial.print("Enter WiFi Password: ");
  wifiPassword = ReadInput();
}
//------------------------------------------------------------------------------------------------
void get_wifiMode() { // Get the WiFi IP address mode (DHCP or Static IP)
  Serial.print("Enter 0 for DHCP or 1 for Static IP: ");
  wifiMode = ReadInput().toInt();
  if (wifiMode > 0) {
    wifiMode = 1;
    Serial.print("\nEnter Static IP Address: ");
    wifiIP = ReadInput();
    Serial.print("\nEnter Subnet Mask: ");
    wifiMask = ReadInput();
    Serial.print("\nEnter Default Gateway: ");
    wifiGateway = ReadInput();
    Serial.print("\nEnter DNS Resolver: ");
    wifiDNS = ReadInput();
  }
}
//------------------------------------------------------------------------------------------------
void get_DeviceName() { // Get the device name (hostname)
  Serial.print("Enter Device Name: ");
  DeviceName = ReadInput();
}
//------------------------------------------------------------------------------------------------
void get_SlaveIP1() { // Get the IP address for slave unit 1
  Serial.print("Enter Slave 1 IP Address: ");
  slaveIP1 = ReadInput();
}
//------------------------------------------------------------------------------------------------
void get_SlaveIP2() { // Get the IP address for slave unit 2
  Serial.print("Enter Slave 2 IP Address: ");
  slaveIP2 = ReadInput();
}
//------------------------------------------------------------------------------------------------
void get_SlaveIP3() { // Get the IP address for slave unit 3
  Serial.print("Enter Slave 3 IP Address: ");
  slaveIP3 = ReadInput();
}
//------------------------------------------------------------------------------------------------
void get_SlaveIP4() { // Get the IP address for slave unit 4
  Serial.print("Enter Slave 4 IP Address: ");
  slaveIP4 = ReadInput();
}
//------------------------------------------------------------------------------------------------
void get_CorrectionFactor() { // Get the correction factor for the temperature sensor
  Serial.print("Enter Correction Factor (-5.0 to 5.0): ");
  CorrectionFactor = ReadInput().toFloat();
}
//------------------------------------------------------------------------------------------------
