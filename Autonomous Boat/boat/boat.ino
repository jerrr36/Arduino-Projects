#include <SD.h>
#include <SPI.h>
#include <TinyGPS++.h>
#include <MPU9250.h>
#include <Math.h>
#include <Wire.h>
#include <SoftwareSerial.h>

//Initializing hardware
MPU9250 mpu;
TinyGPSPlus tinyGPS;
SoftwareSerial ssGPS(9, 8); //9 -> gps tx, arduino rx, 8 -> gps rx, arduino tx
File GPSCOORDS;
File data;


#define R 6371000
#define gpsPort ssGPS

float lat1, lon2;

//GPS Function
float getGPS(lat1, lon1){
  
  float lat2, lon2, t, a, c, d, x, y, B;

  //Initializing lat and lon
  lat2 = tinyGPS.location.lat();
  lon2 = tinyGPS.location.lng();

  /*
   *Add comparison here 
   * 
   */

  
  lat2 = lat2 * PI / 180;
  lon2 = lon2 * PI / 180;

  dLat = lat2 - lat1;
  dLon = lon2 - lon1;

  // Calculating Distance
  float a = math.sin(dLat / 2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dLon / 2) * math.sin(dLon / 2);
  float c = 2 * math.atan2( math.sqrt(a), math.sqrt(1 - a));
  //distance
  float d = R * c;

  //Calculating Bearing Angle

  float x = math.cos(lat2) * math.sin(dLon);
  float y = math.cos(lat1) * math.sin(lat2) - math.sin(lat1) * math.cos(lat2) * math.cos(dLon);
  float B = math.atan2(x, y) * 180 / PI;

  getAngle(B);
  
  
  float coords[] = {lat1, lon1};
  return coords;
}

//Calculating angle difference
void getAngle(float B){
  mpu.update();
  mpu.print();
  xMag, yMag = mpu.getYaw());
  float az = atan2(xMag, yMag) * 180 / PI;
  float angDiff = az - B;

  if (angDiff > 10 || angDiff < -10){
    motorWrite(angDiff);
  }
    
}


//Complete these functions
void motorWrite(angDiff){
 //write angle difference over i2c
}

void writeData(){
  //write all data to SD card
}

void getDepth(){
  //get depth over i2c
}


float readCoords(){
  if (!GPSCOORDS.available()){
    return 1, 1;
  }
  return GPSCOORDS.read()
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  gpsPort.begin(9600);
  mpu.setup();
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  GPSCOORDS = SD.open("coords.txt", FILE_READ);
  data = SD.open("data.txt", FILE_WRITE);
  lat1, lon1 = readCoords();

}

void loop() {
  
  
    
    delay(1000);
    
    
   
   

   
  

}
