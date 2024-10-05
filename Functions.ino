void setSwitchState(AsyncWebServerRequest *request, bool switchState, uint8_t angle) {
    unsigned long currentTime = millis();
    if (currentTime - lastRequestTime < debounceDelay) {
        char jsonResponse[32];
        snprintf(jsonResponse, sizeof(jsonResponse), "{\"error\": \"요청이 너무 빠릅니다.\"}");

        AsyncWebServerResponse *response = request->beginResponse(429, "application/json", jsonResponse);
        request->send(response);
        return; // 요청이 너무 빠르므로 함수 종료
    } else {
        servo.write(angle);
        delay(100);
        servo.write(midAngle);
        isSwitchOn = switchState;
        lastRequestTime = currentTime;

        char jsonResponse[32];
        snprintf(jsonResponse, sizeof(jsonResponse), "{\"isSwitchOn\": %s}", isSwitchOn ? "true" : "false");
        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonResponse);
        request->send(response);

        // Serial.println("스위치 요청이 수신되었습니다");
        // Serial.print("isSwitchOn: ");
        // Serial.println(isSwitchOn);
    }
}

void handleSwitchOn(AsyncWebServerRequest *request) {
    uint8_t onAngleValue;
    memcpy_P(&onAngleValue, &onAngle, sizeof(onAngleValue));
    setSwitchState(request, true, onAngleValue);
}

void handleSwitchOff(AsyncWebServerRequest *request) {
    uint8_t offAngleValue;
    memcpy_P(&offAngleValue, &offAngle, sizeof(offAngleValue));
    setSwitchState(request, false, offAngleValue);
}

void handleSwitch(AsyncWebServerRequest *request) {
    char jsonResponse[32];
    snprintf(jsonResponse, sizeof(jsonResponse), "{\"isSwitchOn\": %s}", isSwitchOn ? "true" : "false");
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", jsonResponse);
    request->send(response);
}

void handleRoot(AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", controlPage);
    request->send(response);

    // Serial.println("루트 요청이 수신되었습니다");
    // Serial.print("isSwitchOn: ");
    // Serial.println(isSwitchOn);
}
