#include <Arduino.h>
#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>

// creating prototypes
void getTemp();
void getHumid();
void getPressure();


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
}

void loop() {

  // calling functions
  getTemp();
  getHumid();
  getPressure();
  
  delay(3000);
}

void getTemp() {
  float temp = HTS.readTemperature(FAHRENHEIT);
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" deg F");
}

void getHumid() {
  float humidity = HTS.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
}

void getPressure() {
  float pressure = BARO.readPressure(PSI);
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" psi");
}