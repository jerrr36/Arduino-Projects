#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include "consts.h"

void wifiConnect(String ssid, String pwd);
void initMQTT(void);
void onMessageCallback(void);

WiFiClientSecure tlsClient;
PubSubClient client(tlsClient);



void setup() {
  // put your setup code here, to run once:
  wifiConnect(SSID, PWD);
}

void loop() {
  // put your main code here, to run repeatedly:
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

void onMessageCallback(){
  return
}

void initMQTT()
{
  tlsClient.setFingerprint(THUMBPRINT);
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(onMessageCallback);
}