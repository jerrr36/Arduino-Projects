#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <TimeLib.h>
#include <time.h>
#include <NtpClientLib.h>
#include "consts.h"


// wifi and mqtt client
WiFiClientSecure tlsClient;
PubSubClient client(tlsClient);

void onMessageCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// function prototypes
void wifiConnect(String ssid, String pwd);
void initMQTT(void);
void connectToIoTHub(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wifiConnect(WIFI_HOST, WIFI_PWD);
  initMQTT();
  connectToIoTHub();

}

void loop() {
  // put your main code here, to run repeatedly:
  client.publish("devices/plant-esp8266-1/messages/events/", "randNum:4");
  delay(60000);
}

void wifiConnect(String ssid, String pwd){
 
  WiFi.begin(ssid, pwd);   // add Wi-Fi networks you want to connect to
 
  Serial.println("Connecting ...");
  
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer 
}

void initMQTT()
{
  tlsClient.setFingerprint(THUMBPRINT);
  client.setServer(MQTT_HOST, 8883);
  client.setCallback(onMessageCallback);
}

void connectToIoTHub() {
  while (!client.connected()) {
      Serial.print("Connecting to MQTT Server ... ");
      if (client.connect(DEVICE_ID, MQTT_USER, MQTT_PASS)) {
        Serial.println("connected.");
        client.subscribe(MQTT_SUB_TOPIC);
      } else {
        Serial.print("failed, status code = ");
        Serial.print(client.state());
        Serial.println(". Try again in 5 seconds.");
        /* Wait 5 seconds before retrying */
        delay(5000);
      }
    }
}