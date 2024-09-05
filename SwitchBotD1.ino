#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

extern IPAddress local_ip;
extern IPAddress gateway;
extern IPAddress subnet;
extern const char *ssid;
extern const char *password;

Servo servo;
const int onAngle = 54;
const int offAngle = 114;
const int midAngle = 90;
bool isSwitchOn = false;
unsigned long lastRequestTime = 0;
const unsigned long debounceDelay = 1000;

AsyncWebServer server(80);
extern const char *controlPage;

void setSwitchState(AsyncWebServerRequest *request, bool switchState, int angle);
void handleSwitchOn(AsyncWebServerRequest *request);
void handleSwitchOff(AsyncWebServerRequest *request);
void handleSwitch(AsyncWebServerRequest *request);
void handleRoot(AsyncWebServerRequest *request);

void setup() {
  Serial.begin(115200);

  servo.attach(D2);
  servo.write(midAngle);

  WiFi.config(local_ip, gateway, subnet);
  WiFi.begin(ssid, password);
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
