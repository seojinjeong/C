#include <AccelStepper.h>
#include <MultiStepper.h>

// Define pins for the first stepper motor (28BYJ-48)
#define IN1_1 13
#define IN2_1 12
#define IN3_1 14
#define IN4_1 27

// Define pins for the second stepper motor
#define IN1_2 15
#define IN2_2 2
#define IN3_2 4
#define IN4_2 16

// Define pins for the third stepper motor
#define IN1_3 17
#define IN2_3 18
#define IN3_3 19
#define IN4_3 21

// Create stepper motor instances for 28BYJ-48
AccelStepper stepper1(AccelStepper::FULL4WIRE, IN1_1, IN3_1, IN2_1, IN4_1);
AccelStepper stepper2(AccelStepper::FULL4WIRE, IN1_2, IN3_2, IN2_2, IN4_2);
AccelStepper stepper3(AccelStepper::FULL4WIRE, IN1_3, IN3_3, IN2_3, IN4_3);

// Create a MultiStepper instance
MultiStepper steppers;

void setup() {
  // Set maximum speed and acceleration for each motor
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);

  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(500);

  stepper3.setMaxSpeed(1000);
  stepper3.setAcceleration(500);

  // Add steppers to MultiStepper
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);

  Serial.begin(115200);
}

void loop() {
  // Define target positions for the steppers
  long positions[3];

  // Set target positions (example: all move 1000 steps forward)
  positions[0] = 512;  // One full rotation for 28BYJ-48
  positions[1] = 1024; // Two full rotations
  positions[2] = 768;  // 1.5 rotations

  // Move all steppers to the target positions
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();

  delay(1000); // Wait for 1 second

  // Reverse the movement
  positions[0] = -512;  // Reverse one full rotation
  positions[1] = -1024; // Reverse two full rotations
  positions[2] = -768;  // Reverse 1.5 rotations

  // Move back to the new target positions
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();

  delay(1000); // Wait for 1 second
}