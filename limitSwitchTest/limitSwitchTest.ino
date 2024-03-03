#include <AccelStepper.h>

AccelStepper stepperX = AccelStepper(1, 3, 6);

#define home_switch 10

long TravelX;
int move_fineiched = 1;
long initial_homing = -1;

void setup() {
  Serial.begin(9600);

  pinMode(10, INPUT_PULLUP);

  delay(5);

  stepperX.setMaxSpeed(100);
  stepperX.setAcceleration(100);

  Serial.println("Stepper is homing ..........");

  while (digitalRead(home_switch)) {
    stepperX.moveTo(initial_homing);
    initial_homing--;
    stepperX.run();
    delay(5);
  }

  stepperX.setCurrentPosition(0);
  stepperX.setMaxSpeed(100);
  stepperX.setAcceleration(100);
  initial_homing = 1;

  while (!digitalRead(home_switch)) {
    stepperX.moveTo(initial_homing);
    stepperX.run();
    initial_homing++;
    delay(5);
  }

  stepperX.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(1000);

  Serial.println("Enter Travel distance(Positive for CW / Negative for CCW and Zero for back to home");

}

void loop() {

}
