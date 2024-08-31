#include <AccelStepper.h>
#include <Servo.h>
#include <MultiStepper.h>

const int step1 = 2;
const int step2 = 12;
const int step3 = 4;
const int step4 = 3;

const int dir1 = 5;
const int dir2 = 13;
const int dir3 = 7;
const int dir4 = 6;

AccelStepper stepper1 = AccelStepper(1, step1, dir1); // base
AccelStepper stepper2 = AccelStepper(1, step2, dir2); // shoulder
AccelStepper stepper3 = AccelStepper(1, step3, dir3); // elbow
AccelStepper stepper4 = AccelStepper(1, step4, dir4); //wrist

MultiStepper steppers;

void setup() {
  Serial.begin(9600);

  // pinMode(step1, OUTPUT);
  // pinMode(step2, OUTPUT);
  // pinMode(step3, OUTPUT);
  // pinMode(step4, OUTPUT);
  // pinMode(dir1, OUTPUT);
  // pinMode(dir2, OUTPUT);
  // pinMode(dir3, OUTPUT);
  // pinMode(dir4, OUTPUT);

  stepper1.setMaxSpeed(500);
  stepper1.setAcceleration(500);
  stepper1.setCurrentPosition(0);

  stepper2.setMaxSpeed(1500);
  stepper2.setAcceleration(500);
  stepper2.setCurrentPosition(0);

  stepper3.setMaxSpeed(500);
  stepper3.setAcceleration(500);
  stepper3.setCurrentPosition(0);

  stepper4.setMaxSpeed(500);
  stepper4.setAcceleration(500);
  stepper4.setCurrentPosition(0);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(stepper4);
}

void loop() {
  long value1[4] = {400, 1800, -600, -600};
  long value2[4] = {0, 0, 0, 0};
  long value3[4] = {-200, -1000, -200, 400};
  steppers.moveTo(value1);
  steppers.runSpeedToPosition();

  delay(2000);

  steppers.moveTo(value2);
  steppers.runSpeedToPosition();

  delay(2000);

  steppers.moveTo(value3);
  steppers.runSpeedToPosition();

  delay(2000);
}