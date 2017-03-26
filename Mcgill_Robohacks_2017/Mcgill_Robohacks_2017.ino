#include <PS3BT.h>
#include <usbhub.h>
#include <Servo.h>


// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

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
  steerServo.attach(servoPin);
}

void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    int leftJoyY = map(PS3.getAnalogHat(LeftHatY), 255, 0, -127, 127);
    int rightJoyX = map(PS3.getAnalogHat(RightHatX), 0, 255, 180, 0);

    if (leftJoyY >= 10) {
      analogWrite(motorOne1, leftJoyY);
      analogWrite(motorOne2, 0);
      steerServo.write(rightJoyX);
    }
    else if (leftJoyY <= -10){
      analogWrite(motorOne1, 0);
      analogWrite(motorOne2, -leftJoyY);
      steerServo.write(rightJoyX);
    }
    else {
      analogWrite(motorOne1, 0);
      analogWrite(motorOne2, 0);
      steerServo.write(rightJoyX);
    }
    delay(20);
    
    
    //analogWrite(motorOne1, 1);
    //analogWrite(motorOne2, leftJoyY);
    //analogWrite(motorTwo1, -leftJoyY);
    //analogWrite(motorTwo2, -leftJoyY);

  }
  
}
