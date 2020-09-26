#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Servo.h>


Servo myServo;
AF_DCMotor motor1(1, MOTOR_64KHZ);
AF_DCMotor motor2(2, MOTOR_64KHZ);

float receiveRequest(){
  //
}
void rotateServo(angle float){
  //Rotating servo based on supplied angle
  //Should prob make this a nice algorithm but I will make it simple for now
  if (angle > 90) {
    servo.write(180);
    delay(4000);
  }
  else if (angle < -90) {
    servo.write(0);
    delay(4000);
  }
  else {
    servo.write(angle);
    delay(2000);
  }
}
void setup() {
  motor1.setSpeed(200);
  motor2.setSpeed(200);
  myServo.attach(9);

}

void loop() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  
  receiveRequest();
  rotateServo(angle);
  
}
