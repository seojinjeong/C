// MultiStepper.pde
// -*- mode: C++ -*-
// Use MultiStepper class to manage multiple steppers and make them all move to 
// the same position at the same time for linear 2d (or 3d) motion.
 
#include <AccelStepper.h>
#include <MultiStepper.h>
 
const int step1 = 2;
const int step2 = 3;
const int step3 = 4;
const int dir1 = 5;
const int dir2 = 6;
const int dir3 = 7;

#define limit1 11
#define limit2 10
#define limit3 9
AccelStepper stepper1 = AccelStepper(1, step1, dir1);
AccelStepper stepper2 = AccelStepper(1, step2, dir2);
AccelStepper stepper3 = AccelStepper(1, step3, dir3);

 
// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper steppers;
 
void setup() {
  Serial.begin(9600);
 
  // Configure each stepper
  stepper1.setMaxSpeed(500);
  stepper1.setAcceleration(1500);
  stepper2.setMaxSpeed(500);
  stepper2.setAcceleration(1500);
  stepper3.setMaxSpeed(500);
  stepper3.setAcceleration(1500);

  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  delay(2);

 
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);

  // long positions[2]; // Array of desired stepper positions

  }
}
 
void loop() {
  long positions[2]; // Array of desired stepper positions
  
  positions[0] = 1000;
  positions[1] = 500;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();// Blocks until all are in position
  delay(1000);
  
  // Move to a different coordinate
  positions[0] = -100;
  positions[1] = 100;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);
}