#include <Arduino.h>
#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"

#define LED_BUILTIN 13

void setup(){
  Serial.begin(115200);
}

void loop(){
  Serial.println("Hello World");
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
}

static unsigned long lastMillis = 0;
void loop()
{
  if (!mqtt->loop())
  {
    mqtt->mqttConnect();
  }

  delay(10); // <- fixes some issues with WiFi stability

  // TODO: Replace with your code here
  if (millis() - lastMillis > 60000)
  {
    lastMillis = millis();
    publishTelemetry(getDefaultSensor());
  }
}
