#include <AccelStepper.h>
// #include <Servo.h>

const int step1 = 2;
const int step2 = 12;
const int step3 = 4;
const int step4 = 3;

const int dir1 = 5;
const int dir2 = 13;
const int dir3 = 7;
const int dir4 = 6;

AccelStepper stepper1 = AccelStepper(1, step1, dir1);
AccelStepper stepper2 = AccelStepper(1, step2, dir2);
AccelStepper stepper3 = AccelStepper(1, step3, dir3);
AccelStepper stepper4 = AccelStepper(1, step4, dir4);

// Servo servo1;

// int pos = 0;

// int speed = 0;

void setup() {
  Serial.begin(9600);

  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(step4, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dir4, OUTPUT);

  // servo1.attach(2);

  stepper1.setMaxSpeed(1000);
  stepper1.moveTo(0);
  stepper1.setAcceleration(500);
  stepper1.setCurrentPosition(0);

  stepper2.setMaxSpeed(1000);
  stepper2.moveTo(0);
  stepper2.setAcceleration(500);
  stepper2.setCurrentPosition(0);

  stepper3.setMaxSpeed(1000);
  stepper3.moveTo(0);
  stepper3.setAcceleration(500);
  stepper3.setCurrentPosition(0);

  stepper4.setMaxSpeed(1000);
  stepper4.moveTo(0);
  stepper4.setAcceleration(500);
  stepper4.setCurrentPosition(0);
}

void loop() {
  stepper1.setSpeed(500);
  stepper2.setSpeed(500);
  stepper3.setSpeed(500);
  stepper4.setSpeed(500);
  stepper1.runSpeed(); 
  stepper2.runSpeed();
  stepper3.runSpeed();
  stepper4.runSpeed();
}
