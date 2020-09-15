#include <MPU9250.h>

#include <math.h>
#include <wire.h>


MPU9250 mpu;

#define R 6371000


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  delay(2000);
  mpu.setup();

}

void loop() {
  
  /*
  float lat1, lat2, lon1, lon2;
  
  Get lat1 and lon1 from gps
  Lat2 and lon2 are from saved data
   
   lat1 = lat1 * PI / 180;
   lat2 = lat2 * PI / 180;
   lon1 = lon1 * PI / 180;
   lon2 = lon2 * PI / 180;
   
   dLat = lat2 - lat1;
   dLon = lon2 - lon1;

   // Calculating Distance
   float a = math.sin(dLat / 2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dLon / 2) * math.sin(dLon / 2);
   float c = 2 * math.atan2( math.sqrt(a), math.sqrt(1 - a));
   float d = R * c;

   //Calculating Bearing Angle

   float x = math.cos(lat2) * sin(dLon);
   float y = math.cos(lat1) * math.sin(lat2) - math.sin(lat1) * math.cos(lat2) * math.cos(dLon);
   float B = math.atan2(x, y) * 180 / PI;
   */
   /*
    Get bearing data from magnetometer
    Get yaw
    Calculate difference
    Send to rudder servo
    
    */

    delay(1000);
    mpu.update();
    mpu.print();
    Serial.println(mpu.getYaw());
    /*
    
    //Magnetometer angle
    float az = atan2(xMag, yMag) * 180 / PI;
    float angDiff = az - B;
     */
    //Pass angle diff to servos and dist to motors
   
   

   
  

}
