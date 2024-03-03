#include <AccelStepper.h>

const int step1 = 2;
const int step2 = 3;
const int step3 = 4;
const int dir1 = 5;
const int dir2 = 6;
const int dir3 = 7;

AccelStepper stepper1 = AccelStepper(1, step1, dir1);
AccelStepper stepper2 = AccelStepper(1, step2, dir2);
AccelStepper stepper3 = AccelStepper(1, step3, dir3);

int speed = 0;

void setup() {
  Serial.begin(9600);

  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);

  stepper1.setMaxSpeed(1000);
  stepper1.moveTo(0);
  stepper1.setAcceleration(1500);
  stepper1.setCurrentPosition(0);

  stepper2.setMaxSpeed(1000);
  stepper2.moveTo(0);
  stepper2.setAcceleration(1500);
  stepper2.setCurrentPosition(0);

  stepper3.setMaxSpeed(1000);
  stepper3.moveTo(0);
  stepper3.setAcceleration(1500);
  stepper3.setCurrentPosition(0);
}

void loop() {
  // digitalWrite(dir1, HIGH);
  // digitalWrite(dir2, HIGH);
  // digitalWrite(dir3, HIGH);

  if(Serial.available()) {
    char c = Serial.read();
    // if(c == 's') {
    //   Serial.println("'s'key is pressed");
    //   stepper1.setSpeed(500);
    //   // stepper1.runSpeed();
    //   // delay(500);
    // }

    if(c == 'q') {
      Serial.println("'q'key is pressed");
      stepper1.setSpeed(-500);
    }

    else if(c == 'a') {
      Serial.println("'a'key is pressed");
      stepper1.setSpeed(500);
    }

    else if(c == 'w') {
      Serial.println("'w'key is pressed");
      stepper2.setSpeed(500);
    }

    else if(c == 's') {
      Serial.println("'s'key is pressed");
      stepper2.setSpeed(-500);
    }

    else if(c == 'e') {
      Serial.println("'e'key is pressed");
      stepper3.setSpeed(-100);
    }

    else if(c == 'd') {
      Serial.println("'d'key is pressed");
      stepper3.setSpeed(100);
    }

    else if(c == '1') {
      Serial.println("'1' key is pressed");
      stepper1.setSpeed(0);
      stepper2.setSpeed(0);
      stepper3.setSpeed(0);
      // stepper1.runSpeed();
      // delay(500);
    }
  }
  stepper1.runSpeed();
  stepper2.runSpeed();
  stepper3.runSpeed();
}
