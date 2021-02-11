#include <Arduino.h>
#include <MPU9250.h>

#define LED 16

void checkAngle(void);
void getAccel(void);

// scl -> d5
// ado -> d6
// sda -> d7
// ncs -> sd3
MPU9250 IMU(SPI,10);
int status;

// timers
unsigned long t1;
unsigned long t2;
float reading;
float angle = 0;


void setup() {
  // serial to display data
  Serial.begin(9600);
  while(!Serial) {}

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);


  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  int cal = IMU.calibrateAccel();
  if (!cal) {
    Serial.println("cal failed");
  }
  t1 = millis();
}

void loop() {
  // read the sensor
  checkAngle();
  //getAccel();
  
}


// checks angle of instrument
void checkAngle() {
  IMU.readSensor();
  reading = IMU.getGyroY_rads();
  if (reading < .0015 && reading > -.0015) {
    reading = 0;
  }
  t2 = millis();
  angle += reading * (t2 - t1) * 180 / (PI * 1000);

  if (angle < -10) {
    digitalWrite(LED, LOW);
  }
  else {
    digitalWrite(LED, HIGH);
  }

  t1 = t2;
  Serial.println(angle);
}

void getAccel(void) {
  IMU.readSensor();
  float x = IMU.getAccelX_mss();
  float y = IMU.getAccelY_mss();
  float z = IMU.getAccelZ_mss();
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.println(z);

}