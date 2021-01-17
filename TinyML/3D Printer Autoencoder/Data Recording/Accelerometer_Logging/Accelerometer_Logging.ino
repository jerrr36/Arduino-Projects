/*
 *  File Created: 1/13/21
 *  Created by: Jeremiah Hoydich
 *  Purpose: Log Accelerometer data from the onboard imu of the Nano 33 BLE Sense 
 *  to an SD card for later analysis
 * 
 */
#include <Arduino_LSM9DS1.h>



float xAcc, yAcc, zAcc, xGyro, yGyro, zGyro;
unsigned long t1, t2, delta;


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(LED_BUILTIN, OUTPUT);
 
    Serial.println("xAcc, yAcc, zAcc, xGyro, yGyro, zGyro, delta");

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

    
    t2 = millis();
    delta = t2 - t1;
    
    Serial.println(String(xAcc) + "," + String(yAcc) + "," + String(zAcc) + "," + String(xGyro) + "," + String(yGyro) + "," + String(zGyro) + "," + String(delta));
    
    digitalWrite(LED_BUILTIN, HIGH);

    t1 = t2;
    }
   else {
      digitalWrite(LED_BUILTIN, LOW);
    }
 

}
