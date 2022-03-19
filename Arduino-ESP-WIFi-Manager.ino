#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

//Variables
int i = 0;
int statusCode;
String content;
String eeprom_ssid;
String eeprom_pass;
String req_ssid;
String req_pass;

// wifi and security variables
const char * access_point_name = "ESP8266 AP";
const char * mdns_wifi_name = "esp8266";
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);

//Function Declaration
bool testWifi(void);
void setupAP(void);
String get_wifi_list(void);
void save_to_eeprom(void);
void clear_wifi_eeprom(void);

//Creating server instance for web
DNSServer dnsServer;
ESP8266WebServer webServer(80);


void setup()
{
  //Initializing if(DEBUG)Serial Monitor
  Serial.begin(115200);
  Serial.println();

  // this is optional if you want you can comment this part
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();

  //Initializing EEPROM
  EEPROM.begin(512);

  // feeding the watchdog
  delay(10);

  // clearing the serial monitor
  Serial.println();
  Serial.println();
  Serial.println("Startup");

  // reading eeprom and geting last wifi credentials
  read_eeprom();

  //  try to connect to wifi for a period of time if it fails will open hotspot
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    createWebServer();
  }

}
void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {
    // Add your program code here which the esp8266 has to perform when it connects to network

  }
  else
  {
    dnsServer.processNextRequest();
    webServer.handleClient();
  }

}
