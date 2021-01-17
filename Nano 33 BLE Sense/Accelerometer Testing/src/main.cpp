#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#define g 9.81

void getAccel(void);
void getGyro(void);
void getMag(void);
float dist(float d);
float getIMUAvg(void);

// initial conditions
float d = 0;
float *t;
float *v;
float xAvg;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // starting IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1) {
      Serial.print("...");
      delay(1500);
    }
  }

  xAvg = getIMUAvg();

  float initT = millis();
  float initV = 0;
  float initX = 0;

  t = &initT;
  v = &initV;
}




void loop() {
  
  //getAccel();
  
  d = dist(d);

  Serial.print("Dist: ");
  Serial.println(d);
  
  

}

// get acceleration data
void getAccel() {
  // x, y, z of acc
  float x, y, z;

  // check if acceleration data is available
  if (IMU.accelerationAvailable()) {

    // if available read data
    IMU.readAcceleration(x, y, z);
    Serial.print("Acceleration x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.print(" z: ");
    Serial.println(z);
  }
}

// get gyro data
void getGyro() {
  // x, y, z gyro readings
  float x, y, z;



  // if gyro available
  if (IMU.gyroscopeAvailable()) {

    // read gyro
    IMU.readGyroscope(x, y, z);

    // print gyro
    Serial.print("Gyro x: ");
    Serial.print(x - xAvg);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.print(" z: ");
    Serial.println(z);
  }
}

// get mag reading
void getMag() {
  // mag x, y, z
  float x, y, z;

  // check if mag is available
  if (IMU.magneticFieldAvailable()) {

    // get gyro data
    IMU.readMagneticField(x, y, z);

    // print gyro
    Serial.print("Mag x");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.print(" z: ");
    Serial.println(z);
  }
}

float dist(float d) {
  // x, y, z of acc
  

  if (IMU.accelerationAvailable()) {
    float x, y, z;
    // if available read data
    IMU.readAcceleration(x, y, z);

    if (x >= -.1 && x <= .1) {
    x = 0.0;
    }

    x /= g;


    // get time delta
   float t2 = millis();
   float delta = t2 - *t;

   *t = t2;


    // calculate velocity and dist
   *v += x * delta;
   d += *v * delta / 2;
  }
  
  

  float a = d;
  return a;
}


float getIMUAvg() {
  float sum = 0;
  int count = 0;
  while (true) {
    if (count == 5 ) {
      
      return sum / count;
    }
    if (IMU.accelerationAvailable()) {
      float x, y, z;
      IMU.readAcceleration(x, y, z);
      sum += x;
      count += 1;
    }
  }
}