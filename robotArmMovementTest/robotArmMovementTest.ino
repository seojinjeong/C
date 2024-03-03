#include <AccelStepper.h>
#include <math.h>
#include <MultiStepper.h>

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
int position1 = 0;
int position2 = 0;
int position3 = 0;

#define limit1 11
#define limit2 10
#define limit3 9

long initial_homing1 = -1;
long initial_homing2 = -1;
long initial_homing3 = -1;


int trans1(float value) {
  return value * 24.4444;
  // Serial.println(degree(90));
}

int trans2(float value) {
  return value * 3;
  // Serial.println(degree(90));
}

float theta1(int x, int y) {
  int L1 = 162;
  int L2 = 132;
  // float alpha = acos((square(L1) + square(x) + square(y) - square(L2))/(2 * L1 * sqrt(square(x) + square(y))));
  float alpha = acos((square(x) + square(y) + square(L1) - square(L2)) / (2 * L1 * sqrt(square(x) + square(y))));
  float theta3 = atan(y/x) + alpha;
  return degrees(theta3);
}   

float theta2(int x, int y) {
  int L1 = 162;
  int L2 = 132;
  // float beta = acos((square(L1) + square(L2) -square(x) -square(y))/(2 * L1 * L2));
  float beta = acos((square(L1) + square(L2) -square(x) -square(y))/(2 * L1 * L2));
  float theta4 = PI - beta;
  return degrees(theta4); 
}

MultiStepper steppers;

void setup() {
  Serial.begin(9600);
  Serial.println("////////////////////");
  Serial.println(theta1(132, 162));
  Serial.println(theta2(132, 162));
  Serial.println("////////////////////");

  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);

//////////////////////////////////////////

  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  delay(2);

  stepper1.setMaxSpeed(500);
  stepper1.setAcceleration(1500);
  stepper2.setMaxSpeed(500);
  stepper2.setAcceleration(1500);
  stepper3.setMaxSpeed(500);
  stepper3.setAcceleration(1500);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  // steppers.addStepper(stepper3);

  Serial.println("Stepper is homing ..........");

  while (digitalRead(limit1)) {
    stepper1.moveTo(initial_homing1);
    initial_homing1--;
    stepper1.run();
    delay(0.2);
  }

  stepper1.setCurrentPosition(0);
  stepper1.setMaxSpeed(500);
  stepper1.setAcceleration(1500);
  initial_homing1 = 1;

  while (!digitalRead(limit1)) {
    stepper1.moveTo(initial_homing1);
    stepper1.run();
    initial_homing1++;
    delay(0.2);
  }

  while (digitalRead(limit2)) {
    stepper2.moveTo(initial_homing2);
    initial_homing2--;
    stepper2.run();
    delay(0.2);
  }

  stepper2.setCurrentPosition(0);
  stepper2.setMaxSpeed(500);
  stepper2.setAcceleration(1500);
  initial_homing2 = 1;

  while (!digitalRead(limit2)) {
    stepper2.moveTo(initial_homing2);
    stepper2.run();
    initial_homing2++;
    delay(0.2);
  }

  while (digitalRead(limit3)) {
    stepper3.moveTo(initial_homing3);
    initial_homing3++;
    stepper3.run();
    delay(0.2);
  }

  stepper3.setCurrentPosition(0);
  stepper3.setMaxSpeed(500);
  stepper3.setAcceleration(1500);
  initial_homing3 = 1;

  while (!digitalRead(limit3)) {
    stepper3.moveTo(initial_homing3);
    stepper3.run();
    initial_homing3--;
    delay(0.2);
  }

///////////////////////////////////////////

  stepper2.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");

  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(1500);

  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(1500);

  stepper3.setMaxSpeed(100);
  stepper3.setAcceleration(1500);

  // stepper2.runToNewPosition(trans1(70));
  // stepper1.runToNewPosition(trans1(90));

  stepper1.setCurrentPosition(trans1(-180));
  stepper2.setCurrentPosition(trans1(-160));
  // stepper3.setCurrentPosition(trans2(+90));

}


////////////////////////////////////////////

// stepper1 -> + : forward, - : backward
// stepper2 -> + : up, - : dowm
// stepper3 -> + : down, - : up

//trans함수 안에 들어가는 값은 상댓값이 아닌 엑추에이터가 가르키는 절대적인 위치를 각도로 표현한 것

/////////////////////////////////////////////

void loop() {
  delay(1000);
  long degrees[2]; // Array of desired stepper positions

  // degrees[0] = trans1(-90);
  // degrees[1] = trans1(-90);
  // // positions[2] = trans2(-20);
  // steppers.moveTo(degrees);
  // steppers.runSpeedToPosition();// Blocks until all are in position
  // delay(1000);
  
  // Move to a different coordinate
  degrees[0] = trans1(-20);
  degrees[1] = trans1(-30);
  // positions[2] = trans2(0);
  steppers.moveTo(degrees);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);

  degrees[0] = trans1(-60);
  degrees[1] = trans1(-140);
  // positions[2] = trans2(0);
  steppers.moveTo(degrees);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);

  // degrees[0] = trans1(-theta1(100, 0));
  // degrees[1] = trans1(-theta1(100, 0));
  // // positions[2] = trans2(0);
  // steppers.moveTo(degrees);
  // steppers.runSpeedToPosition(); // Blocks until all are in position
  // delay(1000);

  // degrees[0] = trans1(-theta1(200, 0));
  // degrees[1] = trans1(-theta1(200, 0));
  // // positions[2] = trans2(0);
  // steppers.moveTo(degrees);
  // steppers.runSpeedToPosition(); // Blocks until all are in position
  // delay(1000);

  
    // if(Serial.available()) {
    //   char c1 = Serial.read();

    //   if (c1 == '1') {
    //     Serial.println("////////////////////");
    //     Serial.println(theta1(200, 0));
    //     Serial.println(theta2(200, 0));
    //     Serial.println("////////////////////");
    //     stepper1.runToNewPosition(trans1(-theta1(200, 0)));
    //     stepper2.runToNewPosition(trans1(-theta2(200, 0)));
    //   }
      
    //   if (c1 == '2') {
    //     Serial.println("////////////////////");
    //     Serial.println(theta1(200, 100));
    //     Serial.println(theta2(200, 100));
    //     Serial.println("////////////////////");
    //     stepper1.runToNewPosition(trans1(-theta1(200, 100)));
    //     stepper2.runToNewPosition(trans1(-theta2(200, 100)));
    //   }

    //   if (c1 == '3') {
    //     Serial.println("////////////////////");
    //     Serial.println(theta1(100, 100));
    //     Serial.println(theta2(100, 100));
    //     Serial.println("////////////////////");
    //     stepper1.runToNewPosition(trans1(-theta1(100, 100)));
    //     stepper2.runToNewPosition(trans1(-theta2(100, 100)));
    //   }

    //   if (c1 == '4') {
    //     Serial.println("////////////////////");
    //     Serial.println(theta1(132, 162));
    //     Serial.println(theta2(132, 162));
    //     Serial.println("////////////////////");
    //     stepper1.runToNewPosition(trans1(-theta1(132, 162)));
    //     stepper2.runToNewPosition(trans1(-theta2(132, 162)));
    //   }
  // }
}
