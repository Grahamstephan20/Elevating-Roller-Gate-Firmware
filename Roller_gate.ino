#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NIDEBUG
#endif

#ifdef ESP32
#include <WiFi.h>
#include <SinricPro.h>
#include <SinricProGarageDoor.h>
#endif

#define WIFI_SSID   "xxxxx"
#define WIFI_PASS   "xxxxx"
#define BAUD_RATE    115200
#define WL 13
#define WH 12
#define UP 4 

#define APP_KEY    "f8208a89-a828-4e96-b81a-9e0379b92cd2"    // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET "1524fe09-0441-419c-b5c2-7bad7c84eb64-7b1bdce8-5d89-4442-a0f8-52b972805d5f" // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define G_OPEN  "648b12f1929949c1da8553aa"  // Should look like "5dc1564130xxxxxxxxxxxxxx"

unsigned long previousMillis = 0;
unsigned long interval = 30000;

void initWiFi () {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    digitalWrite(WL, HIGH);
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  digitalWrite(WL, LOW);
  //delay(1000);
  digitalWrite(WH, HIGH);
  //delay(1000);
}

bool gateOpen(const String &deviceId, bool &state) {
  Serial.printf("device %s turned %s\r\n", deviceId.c_str(), state?"on":"off");
  digitalWrite(UP,HIGH);
  delay(100);
  digitalWrite(UP,LOW);
  return true; // indicate that callback handled correctly
}


void setup() {
  // put your setup code here, to run once
  Serial.begin(BAUD_RATE);
  pinMode(WH, OUTPUT);
  pinMode(WL, OUTPUT);
  pinMode(UP, OUTPUT);
  initWiFi();

  // create and add a switch to SinricPro
  SinricProGarageDoor& myGarageDoor = SinricPro[G_OPEN];
  // set callback function
  myGarageDoor.onDoorState(gateOpen);
  // startup SinricPro
  SinricPro.begin(APP_KEY, APP_SECRET);

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    digitalWrite(WL, HIGH);
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
    digitalWrite(WL, LOW);
    //delay(1000);
    digitalWrite(WH, HIGH);
    //delay(1000);
  }
SinricPro.handle();
}
