#include <Arduino.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0) {
    if ( gps.encode( Serial.read() )) {
      Serial.println(gps.location.lat());
      Serial.println(gps.location.lng());
    }
  }
  delay(1000);
}