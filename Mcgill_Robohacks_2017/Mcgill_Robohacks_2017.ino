#include <PS3BT.h>
#include <usbhub.h>
#include <Servo.h>


// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

int f_Sensor_Trig = 2;
int f_Sensor_Echo = 4;
int S_Sensor_Trig = 7;
int S_Sensor_Echo = 8;
int motorOne1 = 5;
int motorOne2 = 6;
int servoPin = 3;
Servo steerServo;

USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);

void setup() {
  Serial.begin(9600);

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  pinMode(motorOne1, OUTPUT);
  pinMode(motorOne2, OUTPUT);
  pinMode(f_Sensor_Trig, OUTPUT);
  pinMode(f_Sensor_Echo, INPUT);
  pinMode(S_Sensor_Trig, OUTPUT);
  pinMode(S_Sensor_Echo, INPUT);
  steerServo.attach(servoPin);
}

double distanceForward() {
  double duration, distance;
  
  digitalWrite(f_Sensor_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(f_Sensor_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(f_Sensor_Trig, LOW);

  duration = pulseIn(f_Sensor_Echo, HIGH);

  distance = duration/58.2;
  return distance;
}

double distanceBackward() {
  double duration, distance;
  
  digitalWrite(S_Sensor_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(S_Sensor_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(S_Sensor_Trig, LOW);

  duration = pulseIn(S_Sensor_Echo, HIGH);

  distance = duration/58.2;
  return distance;
}



void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
      int leftJoyY = map(PS3.getAnalogHat(LeftHatY), 255, 0, -127, 127);
      int rightJoyX = map(PS3.getAnalogHat(RightHatX), 0, 255, 150, 30);
      
   ///if there is 1m in front and in back of the robot, permit full driver control.
    if (distanceForward() > 100 && distanceBackward() > 100) {
      PS3.setRumbleOff();
  
      if (leftJoyY >= 10) {
        analogWrite(motorOne1, 0);
        analogWrite(motorOne2, leftJoyY);
        steerServo.write(rightJoyX);
      }
      else if (leftJoyY <= -10){
        analogWrite(motorOne1, -leftJoyY);
        analogWrite(motorOne2, 0);
        steerServo.write(rightJoyX);
      }
      else {
        analogWrite(motorOne1, 0);
        analogWrite(motorOne2, 0);
        steerServo.write(rightJoyX);
      }
    }

    //if there is less than 1m in front but more than 1m in back, only allow reverse drive.
    else if (distanceForward() < 100 && distanceBackward() > 100) {
      PS3.setRumbleOn(RumbleLow);
      
      if (leftJoyY >= 10) {
        analogWrite(motorOne1, 0);
        analogWrite(motorOne2, 0);
        steerServo.write(rightJoyX);
      }
      else if (leftJoyY <= -10){
        analogWrite(motorOne1, -leftJoyY);
        analogWrite(motorOne2, 0);
        steerServo.write(rightJoyX);
      }
      else {
        analogWrite(motorOne1, 0);
        analogWrite(motorOne2, 0);
        steerServo.write(rightJoyX);
      }
    }

    //if there is less than 1m in back but more than 1m in front, only allow forward drive.
    else if (distanceForward() > 100 && distanceBackward() < 100) {
      PS3.setRumbleOn(RumbleLow);

      if (leftJoyY >= 10) {
        analogWrite(motorOne1, 0);
        analogWrite(motorOne2, leftJoyY);
        steerServo.write(rightJoyX);
      }
      else if (leftJoyY <= -10){
        analogWrite(motorOne1, 0);
        analogWrite(motorOne2, 0);
        steerServo.write(rightJoyX);
      }
      else {
        analogWrite(motorOne1, 0);
        analogWrite(motorOne2, 0);
        steerServo.write(rightJoyX);
      }
    }

    //if there is less than 1m in front and in back, then don't move.
    else if (distanceForward() < 100 && distanceBackward() < 100){
      Serial.print(distanceForward());
      PS3.setRumbleOn(RumbleHigh);
      
      analogWrite(motorOne1, 0);
      analogWrite(motorOne2, 0);
      steerServo.write(rightJoyX);
    }

      /*int leftJoyY = map(PS3.getAnalogHat(LeftHatY), 255, 0, -100, 100);
      int rightJoyX = map(PS3.getAnalogHat(RightHatX), 0, 255, 150, 30);

      PS3.setRumbleOff();
  
      if (leftJoyY >= 10) {
        analogWrite(motorOne1, 0);
        analogWrite(motorOne2, leftJoyY);
        steerServo.write(rightJoyX);
      }
      else if (leftJoyY <= -10){
        analogWrite(motorOne1, -leftJoyY);
        analogWrite(motorOne2, 0);
        steerServo.write(rightJoyX);
      }
      else {
        analogWrite(motorOne1, 0);
        analogWrite(motorOne2, 0);
        steerServo.write(rightJoyX);
      }*/
      
  }
  
}
