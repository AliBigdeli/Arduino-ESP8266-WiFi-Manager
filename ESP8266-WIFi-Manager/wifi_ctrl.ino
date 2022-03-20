//function to test connecting to wifi with at least a try for each second
bool testWifi(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(eeprom_ssid.c_str(), eeprom_pass.c_str());
  int connection_counter = 0;
  Serial.println("Waiting for WiFi to connect");

  // trying to connect to wifi for at least 20 seconds
  while ( connection_counter < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println();
      return true;
    }
    delay(500);
    Serial.print("*");
    connection_counter++;
  }

  // failed to connect after the period
  Serial.println("");
  Serial.println("Connection timed out, opening AP or Hotspot");
  return false;
}


void setupAP(void)
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(access_point_name);
  Serial.println("Initializing_Wifi_accesspoint");
}

String get_wifi_list() {
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  String st  = "";
  for (int i = 0; i < n; ++i)
  {
    st += "<div><a href='#p' onclick='c(this)'>";
    st += WiFi.SSID(i);
    st += "</a>&nbsp;";
    st += "<span class='q l'>";
    st += WiFi.RSSI(i);
    st += " ";
    st += (WiFi.encryptionType(i) == AUTH_OPEN) ? " " : "(*)";
    st += "</span></div>";
  }
  return st;
}
