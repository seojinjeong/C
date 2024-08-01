#include <AccelStepper.h>
#include <math.h>
#include <MultiStepper.h>

const int step1 = 2;
const int step2 = 3;
const int step3 = 4;
const int step4 = 12;
const int dir1 = 5;
const int dir2 = 6;
const int dir3 = 7;
const int dir4 = 13;

AccelStepper stepper1 = AccelStepper(1, step1, dir1);
AccelStepper stepper2 = AccelStepper(1, step2, dir2);
AccelStepper stepper3 = AccelStepper(1, step3, dir3);
AccelStepper stepper4 = AccelStepper(1, step4, dir4);

const float L1 = 10.0;
const float L2 = 10.0;
const float L3 = 10.0;
const float L4 = 10.0;

float val = 0;
float thetas[4] = {0, 0, 0, 0};  // Global array to store theta values

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

MultiStepper steppers;

// -----------------------------------------------

float rad2deg(float value) {
  return value * 180 / PI;
}

float trans(float value) {
  return value * 24.4444;
}

void calculate_inverse_kinematics(float x, float y, float z, float psi) {
  float alpha = 0;
  float beta = 0;
  float lambda = 0;
  float omega = 0;

  float theta1 = 0;
  float theta2 = 0;
  float theta3 = 0;
  float theta4 = 0;

  float B = 0;
  float C = 0;

  float Ex = 0;
  float Ey = 0;

  Ex = sqrt(sq(x) + sq(y));
  Ey = z;

  B = sqrt(sq(Ex - L4 * sin(psi)) + sq(Ey - L1 + L4 * sin(psi)));
  C = sqrt(sq(Ex) + sq(z - L1));

  alpha = acos((L2 * L2 + B * B - L3 * L3) / (2 * B * L2));
  beta = acos((L2 * L2 + L3 * L3 - B * B) / (2 * L2 * L3));
  lambda = acos((B * B + C * C - L4 * L4) / (2 * B * C));
  omega = atan2((z - L1), sqrt(x * x + y * y));

  theta1 = atan2(y, x);
  theta2 = PI / 2 - (alpha + lambda + omega);
  theta3 = PI - beta;
  theta4 = PI - (psi + theta2 + theta3);

  thetas[0] = theta1;
  thetas[1] = theta2;
  thetas[2] = -theta3;
  thetas[3] = theta4;
}

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

  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);
  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(500);
  stepper3.setMaxSpeed(1000);
  stepper3.setAcceleration(500);
  stepper4.setMaxSpeed(1000);
  stepper4.setAcceleration(500);

  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);
  stepper4.setCurrentPosition(0);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(stepper4);

  // Serial.println("Stepper is homing ..........");

  // while (digitalRead(limit1)) {
  //   stepper2.moveTo(initial_homing1);
  //   initial_homing1++;
  //   stepper2.run();
  //   delay(0.2);
  // }

  // initial_homing1 = 1;

  // while (!digitalRead(limit1)) {
  //   stepper2.moveTo(initial_homing1);
  //   stepper2.run();
  //   initial_homing1--;
  //   delay(0.2);
  // }

  // while (digitalRead(limit2)) {
  //   stepper3.moveTo(initial_homing2);
  //   initial_homing2++;
  //   stepper3.run();
  //   delay(0.2);
  // }

  // initial_homing2 = 1;

  // while (!digitalRead(limit2)) {
  //   stepper3.moveTo(initial_homing2);
  //   stepper3.run();
  //   initial_homing2--;
  //   delay(0.2);
  // }

  // while (digitalRead(limit3)) {
  //   stepper4.moveTo(initial_homing3);
  //   initial_homing3--;
  //   stepper4.run();
  //   delay(0.2);
  // }

  // initial_homing3 = 1;

  // while (!digitalRead(limit3)) {
  //   stepper4.moveTo(initial_homing3);
  //   stepper4.run();
  //   initial_homing3++;
  //   delay(0.2);
  // }

  // stepper2.setCurrentPosition(trans(-90));
  // stepper3.setCurrentPosition(trans(-90));
  // stepper4.setCurrentPosition(trans(-90));
}

void loop() {
  long degrees[4];

  calculate_inverse_kinematics(10, 10, 10, 0);

  degrees[0] = trans(rad2deg(thetas[0]));
  degrees[1] = trans(rad2deg(thetas[1]));
  degrees[2] = trans(rad2deg(thetas[2]));
  degrees[3] = trans(rad2deg(thetas[3]));

  steppers.moveTo(degrees);
  steppers.runSpeedToPosition();

  // Serial.print("Returned theta1: ");
  // Serial.println(rad2deg(thetas[0]));
  // Serial.print("Returned theta2: ");
  // Serial.println(rad2deg(thetas[1]));
  // Serial.print("Returned theta3: ");
  // Serial.println(rad2deg(thetas[2]));
  // Serial.print("Returned theta4: ");
  // Serial.println(rad2deg(thetas[3]));
}
