#include <Arduino.h>

// nano 33 ble sense onboard sensors
#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_APDS9960.h>
#include <Arduino_LSM9DS1.h>
#include <PDM.h>

// on board led
#define RED 22     
#define BLUE 24     
#define GREEN 23


// creating prototypes
void getTemp(void);
void getHumid(void);
void getPressure(void);
void getColor(void);
void getProx(void);
void getAccel(void);
void getGyro(void);
void getMag(void);
void getGesture(void);
void getSound(void);
void onPDMdata(void);
void runLEDS(void); // not working

short sampleBuffer[256];
volatile int samplesRead;

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
    while (1) {
      Serial.print("...");
      delay(1500);
    }
  }

  // callback for receiving data
  PDM.onReceive(onPDMdata);

  // starting microphone
  if(!PDM.begin(1, 16000)) {
    Serial.println("Failed to start mic");
    while(1);
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
  getAccel();
  //getGesture();
  delay(10);
  //getSound(); 
  //runLEDS();
}

// get temperatuer
void getTemp() {
  // getting temp
  float temp = HTS.readTemperature(FAHRENHEIT);

  // printing temp
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" deg F");
  
}

// get humidity
void getHumid() {
  // getting humidity
  float humidity = HTS.readHumidity();

  // printing humidity
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
}

// get pressure
void getPressure() {
  // reading pressure
  float pressure = BARO.readPressure(PSI);

  // printing pressure
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" psi");
}

// get color data
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

// get distance
void getProx() {
  // check if prox is available
  if (APDS.proximityAvailable()) {
    int prox = APDS.readProximity();
    
    // printing proximity
    Serial.print("Dist: ");
    Serial.println(prox);
  } 
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
    Serial.print(x);
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

// checking gesture
void getGesture() {
  // check if a gesture reading is available
  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {
      case GESTURE_UP:
        Serial.println("Detected UP gesture");
        break;

      case GESTURE_DOWN:
        Serial.println("Detected DOWN gesture");
        break;

      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        break;

      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        break;

      default:
        // ignore
        break;
    }
  }
}

// getting mic data
void getSound() {
  // wait for samples to be read
  if (samplesRead) {

    // print samples to the serial monitor or plotter
    for (int i = 0; i < samplesRead; i++) {
      Serial.println(sampleBuffer[i]);
    }

    // clear the read count
    samplesRead = 0;
  }
}

// callback for mic
void onPDMdata() {
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}

// running rgb led
void runLEDS() {
  digitalWrite(RED, LOW); // turn the LED off by making the voltage LOW
  delay(1000);            // wait for a second
  digitalWrite(GREEN, LOW);
  delay(1000);  
  digitalWrite(BLUE, LOW);
  delay(1000);  
  digitalWrite(RED, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(1000);                         
  digitalWrite(GREEN, HIGH);
  delay(1000);  
  digitalWrite(BLUE, HIGH);
  delay(1000);
}