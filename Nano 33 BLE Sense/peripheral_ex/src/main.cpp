#include <Arduino.h>

// nano 33 ble sense onboard sensors
#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_APDS9960.h>
#include <Arduino_LSM9DS1.h>


// creating prototypes
void getTemp(void);
void getHumid(void);
void getPressure(void);
void getColor(void);
void getProx(void);
void getAccel(void);
void getGyro(void);
void getMag(void);


int r = 0, g = 0, b = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // starting temperature and humidity sensor
  if(!HTS.begin()) {
    Serial.println("Failed to init hts sensor");
  }

  // starting pressure sensor
  if(!BARO.begin()) {
    Serial.println("Failed to init pressure sensor");
  }

  // starting pressure sensor
  if(!APDS.begin()) {
    Serial.println("Failed to init color/gesture sensor");
  }

  // starting IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  
}

void loop() {
  //getTemp();
  //getHumid();
  //getPressure();
  //getColor();
  //getProx();
  //getGyro();
  //getMag();
  //getAccel();
  delay(1000);
}

void getTemp() {
  // getting temp
  float temp = HTS.readTemperature(FAHRENHEIT);

  // printing temp
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" deg F");
  
}

void getHumid() {
  // getting humidity
  float humidity = HTS.readHumidity();

  // printing humidity
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
}

void getPressure() {
  // reading pressure
  float pressure = BARO.readPressure(PSI);

  // printing pressure
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" psi");
}

void getColor() {
  int r, g, b;
  // seeing if color is available
  if (APDS.colorAvailable()) {
    APDS.readColor(r, g, b);

    // printing the color read
    Serial.print("R: ");
    Serial.print(r);
    Serial.print(" G: ");
    Serial.print(g);
    Serial.print(" B: ");
    Serial.print(b);
  }
}

void getProx() {
  // check if prox is available
  if (APDS.proximityAvailable()) {
    int prox = APDS.readProximity();
    
    // printing proximity
    Serial.print("Dist: ");
    Serial.println(prox);
  } 
}

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

void getGyro() {
  // x, y, z gyro readings
  float x, y, z;



  // if gyro available
  if (IMU.gyroscopeAvailable()) {

    // read gyro
    IMU.readGyroscope(x, y, z);

    // print gyro
    Serial.print("Gyro x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.print(" z: ");
    Serial.println(z);
  }
}

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