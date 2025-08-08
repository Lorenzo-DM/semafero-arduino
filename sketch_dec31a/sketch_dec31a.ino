// Arduino_JSON - Version: Latest
#include <Arduino_JSON.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#define RED_LED 3
#define YELLOW_LED 4
#define GREEN_LED 5

// Replace with your network credentials
const char *ssid = "REPLACE_WITH_SSID";
const char *password = "REPLACE_WITH_PASSWORD";

String SemaferoAPI = "https://semafero.vercel.app/api/next-green";

void setup()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  int retries = 20;
  int i = 0;
  bool connected = false;
  while (i <= retries && !connected)
  {
    Serial.print('.');
    delay(500);
    connected = WiFi.status() == WL_CONNECTED;
    i++;
  }

  if (connected)
  {
    performRealHours();
  }
}

void loop()
{
  turnOn(GREEN_LED, 50000);  // 50 seconds
  turnOn(YELLOW_LED, 10000); // 10 seconds
  turnOn(RED_LED, 450000);   // 450 seconds
}

void performRealHours()
{
  Serial.print("get time to next green");
  String payload = getPayload(SemaferoAPI);
  Serial.println(payload);
  JSONVar myObject = JSON.parse(payload);
  if (JSON.typeof(myObject) == "undefined")
  {
    Serial.println("Parsing input failed!");
    return;
  }
  if (!myObject.hasOwnProperty("timeToGreen"))
  {
    return;
  }
  long timeToGreen = (long)myObject["timeToGreen"];

  turnOn(RED_LED, timeToGreen);
}

String getPayload(String endpoint)
{
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  String payload = "";
  Serial.print("[HTTPS] begin...\n");
  if (https.begin(*client, endpoint))
  {
    Serial.print("[HTTPS] GET...\n");
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        payload = https.getString();
      }
    }
    else
    {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  }
  return payload;
}

void turnOn(int color, long milliseconds)
{
  digitalWrite(color, HIGH);
  delay(milliseconds);
  digitalWrite(color, LOW);
}
