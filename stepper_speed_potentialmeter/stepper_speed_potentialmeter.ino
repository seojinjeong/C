#include <AccelStepper.h>

#define DIR 2
#define STEP 3

int speed1;

AccelStepper stepper1 = AccelStepper(1, STEP, DIR);

void setup() {
  stepper1.setMaxSpeed(500);

}

void loop() {
  speed1 = map(analogRead(A0), 0, 1023, -500, 500);
  stepper1.setSpeed(speed1);
  stepper1.runSpeed();
}
 