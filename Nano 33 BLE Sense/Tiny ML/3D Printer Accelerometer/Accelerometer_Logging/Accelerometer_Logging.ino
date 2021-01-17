/*
 *  File Created: 1/13/21
 *  Created by: Jeremiah Hoydich
 *  Purpose: Log Accelerometer data from the onboard imu of the Nano 33 BLE Sense 
 *  to an SD card for later analysis
 * 
 */

#include <SPI.h>
#include "SdFat.h"
#include <Arduino_LSM9DS1.h>

#define SD_FAT_VERSION 2
#define SD_CS_PIN 4

SdFat SD;
File myFile;


float xAcc, yAcc, zAcc, xGyro, yGyro, zGyro;
unsigned long t1, t2;


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("log.csv", FILE_WRITE);
  if (myFile) {
    myFile.println("xAcc, yAcc, zAcc, xGyro, yGyro, zGyro");
    myFile.close();
  }

  
  
  
// cs -> D4
// sck -> D13
// miso -> D12
// mosi -> D11

  if (!IMU.begin()) {
    Serial.println("IMU failed to initialize");
    while (1);
  }

  t1 = millis();
}




void loop() {
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro);

    
    myFile = SD.open("log.csv", FILE_WRITE);
    //Serial.println(String(xAcc) + "," + String(yAcc) + "," + String(zAcc) + "," + String(xGyro) + "," + String(yGyro) + "," + String(zGyro));
    if (myFile) {
      t2 = millis();
      unsigned long delta = t2 - t1;
      
      myFile.println(String(xAcc) + "," + String(yAcc) + "," + String(zAcc) + "," + String(xGyro) + "," + String(yGyro) + "," + String(zGyro) + "," + String(delta));
      digitalWrite(LED_BUILTIN, HIGH);
      myFile.close();

      t1 = t2;
    }
    else {
      Serial.println("Issue with file");
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

}
