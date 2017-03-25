#include <PS3BT.h>
#include <usbhub.h>
#include <Servo.h>


// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

int motorOne;
int motorTwo;
int servoPin = 6;
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

  pinMode(motorOne, OUTPUT);
  pinMode(motorTwo, OUTPUT);
  steerServo.attach(servoPin);
}

void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    int leftJoyY = map(PS3.getAnalogHat(LeftHatY), 255, 0, -127, 127);
    int rightJoyX = map(PS3.getAnalogHat(RightHatX), 0, 255, -127, 127);

    steerServo.write(map(PS3.getAnalogHat(RightHatX), 0, 255, 0, 180));

    if (abs(leftJoyY) <= 10){
      analogWrite(motorOne, 0);
      analogWrite(motorTwo, 0);
    }
    else {
      analogWrite(motorOne, leftJoyY);
      analogWrite(motorTwo, -leftJoyY);
    }
  }
  
}
