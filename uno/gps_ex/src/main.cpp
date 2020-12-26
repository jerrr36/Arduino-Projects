#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial ss(8,9);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ss.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  if (gps.location.isUpdated()) {
    Serial.print(gps.location.lat());
    Serial.print(", ");
    Serial.println(gps.location.lng());
  } 
  
  delay(2000);
}