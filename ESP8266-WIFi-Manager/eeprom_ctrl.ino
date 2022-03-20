void read_eeprom() {
  // Read eeprom for ssid and password
  Serial.println("Reading EEPROM ssid");
  for (int i = 0; i < 32; ++i)
  {
    eeprom_ssid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(eeprom_ssid);
  Serial.println("Reading EEPROM pass");

  for (int i = 32; i < 96; ++i)
  {
    eeprom_pass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(eeprom_pass);
}

void clear_wifi_eeprom() {
  Serial.println("clearing eeprom");
  for (int i = 0; i < 96; ++i) {
    EEPROM.write(i, 0);
  }
}

void save_to_eeprom() {
  clear_wifi_eeprom();
  Serial.println(req_ssid);
  Serial.println("");
  Serial.println(req_pass);
  Serial.println("");

  Serial.println("writing eeprom ssid:");
  for (int i = 0; i < req_ssid.length(); ++i)
  {
    EEPROM.write(i, req_ssid[i]);
    Serial.print("Wrote: ");
    Serial.println(req_ssid[i]);
  }
  Serial.println("writing eeprom pass:");
  for (int i = 0; i < req_pass.length(); ++i)
  {
    EEPROM.write(32 + i, req_pass[i]);
    Serial.print("Wrote: ");
    Serial.println(req_pass[i]);
  }
  EEPROM.commit();
}
