#include <Arduino.h>
#include <ArduinoJson.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_AHTX0.h>

#define SSID "SSID"
#define PASSWORD "PASSWORD"
#define HOST "192.XXX.XXX.XXX"
#define PORT 3000

#define PIR_PIN 23

#define LAMP1_PIN 4
#define LAMP2_PIN 5
#define LAMP3_PIN 18
#define LAMP4_PIN 19

#define FAN1_PIN 25
#define FAN2_PIN 26

#define GENERAL1_PIN 32
#define GENERAL2_PIN 33

WiFiMulti wifiMulti;
HTTPClient http;

Adafruit_AHTX0 aht20;

sensors_event_t aht20TempEvt, aht20HumEvt;

bool pirState = false;
float temperature = 0;
float humidity = 0;

void setup() {
  Serial.begin(115200);

  if (!aht20.begin()) {
    Serial.println("Nao foi possivel encontrar um sensor AHT20, verifique as conexoes!");
    while (1);
  }

  pinMode(PIR_PIN, INPUT);

  pinMode(LAMP1_PIN, OUTPUT);
  pinMode(LAMP2_PIN, OUTPUT);
  pinMode(LAMP3_PIN, OUTPUT);
  pinMode(LAMP4_PIN, OUTPUT);

  pinMode(FAN1_PIN, OUTPUT);
  pinMode(FAN2_PIN, OUTPUT);

  pinMode(GENERAL1_PIN, OUTPUT);
  pinMode(GENERAL2_PIN, OUTPUT);

  wifiMulti.addAP(SSID, PASSWORD);
  http.setReuse(true);
}

bool digitalReadBool(int pin) {
  return digitalRead(pin) == HIGH;
}

void readSensors() {
  pirState = digitalReadBool(PIR_PIN);
  aht20.getEvent(&aht20HumEvt, &aht20TempEvt);

  if (isnan(aht20TempEvt.temperature) || isnan(aht20HumEvt.relative_humidity)) {
    Serial.println("Falha na leitura do aht20!");
    return;
  }

  temperature = aht20TempEvt.temperature;
  humidity = aht20HumEvt.relative_humidity;
}

void sendSensorData() {
  JsonDocument doc;
  doc["pir"] = pirState;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  Serial.print("[HTTP] POST /sensors body: ");
  serializeJsonPretty(doc, Serial);

  http.begin(HOST, PORT, "/sensors");
  http.addHeader("Content-Type", "application/json");

  String requestBody;
  serializeJson(doc, requestBody);

  int httpCode = http.POST(requestBody);
  if (httpCode > 0) {
    Serial.printf("[HTTP] POST /sensors code: %d\n", httpCode);
  } else {
    Serial.printf("[HTTP] POST /sensors error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void writeIfKeyExists(JsonDocument& doc, const char* key, uint8_t pin) {
  if (doc.containsKey(key) && !doc[key].isNull()) {
    digitalWrite(pin, doc[key] ? HIGH : LOW);
  }
}

void syncActuatorStates() {
  JsonDocument doc;

  doc["lamp1"] = digitalReadBool(LAMP1_PIN);
  doc["lamp2"] = digitalReadBool(LAMP2_PIN);
  doc["lamp3"] = digitalReadBool(LAMP3_PIN);
  doc["lamp4"] = digitalReadBool(LAMP4_PIN);

  doc["fan1"] = digitalReadBool(FAN1_PIN);
  doc["fan2"] = digitalReadBool(FAN2_PIN);

  doc["general1"] = digitalReadBool(GENERAL1_PIN);
  doc["general2"] = digitalReadBool(GENERAL2_PIN);

  Serial.print("[HTTP] POST /actuators/state body: ");
  serializeJsonPretty(doc, Serial);

  http.begin(HOST, PORT, "/actuators/state");
  http.addHeader("Content-Type", "application/json");

  String requestBody;
  serializeJson(doc, requestBody);

  int httpCode = http.POST(requestBody);
  if (httpCode > 0) {
    Serial.printf("[HTTP] POST /actuators/state code: %d\n", httpCode);

    String response = http.getString();
    JsonDocument resDoc;
    DeserializationError error = deserializeJson(resDoc, response);
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
    } else {
      Serial.print("[HTTP] POST /actuators/state response: ");
      serializeJsonPretty(resDoc, Serial);

      writeIfKeyExists(resDoc, "lamp1", LAMP1_PIN);
      writeIfKeyExists(resDoc, "lamp2", LAMP2_PIN);
      writeIfKeyExists(resDoc, "lamp3", LAMP3_PIN);
      writeIfKeyExists(resDoc, "lamp4", LAMP4_PIN);

      writeIfKeyExists(resDoc, "fan1", FAN1_PIN);
      writeIfKeyExists(resDoc, "fan2", FAN2_PIN);

      writeIfKeyExists(resDoc, "general1", GENERAL1_PIN);
      writeIfKeyExists(resDoc, "general2", GENERAL2_PIN);
    }
  } else {
    Serial.printf("[HTTP] POST /actuators/state error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void loop() {
  readSensors();

  if (wifiMulti.run() == WL_CONNECTED) {
    sendSensorData();
    syncActuatorStates();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(2500);
}
