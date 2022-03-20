
String temp_page(String body, bool back = true) {
  String html = "<!DOCTYPE HTML> <head> <meta http-equiv='content-type' content='text/html'; charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'> <style> div, input { padding: 5px 5px 5px 5px; background: #ffffff; } input,select { width: 96%; height: 40px; margin: 8px 0; display: inline-block; border: 1px solid #ccc; border-radius: 4px; font-size: 15px } select { width: 100%;} button:hover, input[type=submit]:hover { background-color: #3d85c6; } h1 { text-align: center; } body { text-align: center; font-family: verdana; background: #34b1eb; font-size: 14px; } input[type=submit],input[type=button],input[type=reset], button { height: 50px; border: 0; border-radius: 0.3rem; background-color: #34b1eb; color: #ffffff; line-height: 2.4rem; font-size: 1.2rem; width: 100%; cursor: pointer; margin: 8px 0; } .panel { background: #fff; max-height: auto; width: 280px; margin: 75px auto; padding: 30px; border-radius: 8px; text-align:left; display:inline-block; } </style> </head> <body> <div class='panel'>";
  html += body;
  if (back == true) {
    html += "<form action='/' method='get'><button>Back</button></form>";
  }
  html += "</div> </body> </html>";
  return html;
}
String config_panel = "<h1>ConfigPanel</h1> <form action='/wifi' method='get'><button>WiFi Config</button></form> </form><form action='/system' method='get' ><button>System</button></form>  <form action='/about' method='get' ><button>About</button></form>";

void handleWifi() {  
  String wifi_panel  = "<script>function c(l){document.getElementById('ssid').value=l.innerText||l.textContent;document.getElementById('pass').focus();}</script>";
  wifi_panel += "<h1>Nearby Wireless Networks</h1>";
  wifi_panel += get_wifi_list();
  wifi_panel += "<form method='POST' action='/wifi_config'> <input id='ssid' name='ssid' length='32' placeholder='SSID' required='required'> <input id='pass' name='pass' length='32' type='password' placeholder='Password' required='required'> <button type='submit'>save</button> <button class='b' onclick='document.location.reload(); return false;'>scan</button></form>";
  webServer.send(200, "text/html",  temp_page(wifi_panel));
};

void handleReset() {
  clear_wifi_eeprom();  
  ESP.reset();
}

void handleReboot() {  
  ESP.reset();
}

void handleSetting() {
  req_ssid = webServer.arg("ssid");
  req_pass = webServer.arg("pass");
  if (req_ssid.length() > 0 && req_pass.length() > 0) {
    save_to_eeprom();
    content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
    statusCode = 200;
    ESP.reset();
  } else {
    content = "{\"Error\":\"404 not found\"}";
    statusCode = 404;
    Serial.println("Sending 404");
  }
  webServer.sendHeader("Access-Control-Allow-Origin", "*");
  webServer.send(statusCode, "application/json", content);
}

void handleSystem() {
  String system_panel = "<script>function myconfirmation(mode) {if (mode == 1){var ans = confirm('Are you sure about rebooting ?');if (ans){console.log('going to reboot');window.location.href = '/reboot';return false;}}else if (mode == 0){var ans = confirm('Are you sure about reseting all configurations ?');if (ans == true){window.location.href = '/reset';return false;}}}</script><h1>System </h1><button onclick='return myconfirmation(1)'>Reboot</button><button onclick='return myconfirmation(0)'>Reset</button>";
  webServer.send(200, "text/html", temp_page(system_panel, true));
}

void handleAbout() {
  String about_panel = "<h1>Created by: <br> Ali Bigdeli <br> Use it well</h1>";
  webServer.send(200, "text/html", temp_page(about_panel, true));
}

void handleNotFound() {
  webServer.send(200, "text/html", temp_page(config_panel, false));
}

void handleRoot() {
  webServer.send(200, "text/html", temp_page(config_panel, false));
}

void createWebServer()
{
  setupAP();
  //Setup the DNS server redirecting all the domains to the apIP
  //  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.on("/", HTTP_GET, handleRoot);
  webServer.on("/system", HTTP_GET, handleSystem);
  webServer.on("/about", HTTP_GET, handleAbout);
  webServer.on("/wifi", HTTP_GET, handleWifi);  
  webServer.on("/reboot", HTTP_GET, handleReboot);
  webServer.on("/reset", HTTP_GET, handleReset);
  webServer.on("/wifi_config", HTTP_POST, handleSetting);
  webServer.onNotFound(handleNotFound);
  webServer.begin();
  Serial.println("Server started");
}
