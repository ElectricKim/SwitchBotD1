#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

extern const uint8_t local_ip[4] PROGMEM;
extern const uint8_t gateway[4] PROGMEM;
extern const uint8_t subnet[4] PROGMEM;
extern const char ssid[] PROGMEM;
extern const char password[] PROGMEM;

Servo servo;
const uint8_t onAngle PROGMEM = 54;
const uint8_t offAngle PROGMEM = 114;
const uint8_t midAngle PROGMEM = 90;
bool isSwitchOn = false;
unsigned long lastRequestTime = 0;
const unsigned long debounceDelay = 1000;

AsyncWebServer server(80);
extern const char controlPage[] PROGMEM;

void setSwitchState(AsyncWebServerRequest *request, bool switchState, uint8_t angle);
void handleSwitchOn(AsyncWebServerRequest *request);
void handleSwitchOff(AsyncWebServerRequest *request);
void handleSwitch(AsyncWebServerRequest *request);
void handleRoot(AsyncWebServerRequest *request);

void setup() {
  Serial.begin(115200);
  servo.attach(D2);

  uint8_t midAngleValue;
  memcpy_P(&midAngleValue, &midAngle, sizeof(midAngleValue));
  servo.write(midAngleValue);

  uint8_t ip[4];
  memcpy_P(ip, local_ip, sizeof(ip));
  WiFi.config(IPAddress(ip[0], ip[1], ip[2], ip[3]), 
              IPAddress(gateway[0], gateway[1], gateway[2], gateway[3]), 
              IPAddress(subnet[0], subnet[1], subnet[2], subnet[3]));
  
  char ssidBuffer[32];
  char passwordBuffer[32];
  strcpy_P(ssidBuffer, ssid);
  strcpy_P(passwordBuffer, password);
  
  WiFi.begin(ssidBuffer, passwordBuffer);
  // Serial.print("WiFi에 연결 중...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
  }

  // Serial.println();
  // Serial.print("WiFi에 연결되었습니다. IP 주소: ");
  // Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/switch", HTTP_GET, handleSwitch);
  server.on("/switch-on", HTTP_GET, handleSwitchOn);
  server.on("/switch-off", HTTP_GET, handleSwitchOff);
  server.begin();
  // Serial.println("HTTP 서버가 시작되었습니다");
}

void loop() {}
