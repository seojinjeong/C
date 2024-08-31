#include <AccelStepper.h>
#include <Servo.h>
#include <SoftwareWire.h>

SoftwareWire myWire(A8, A9);  // SDA = A8, SCL = A9
#define AS5600_ADDRESS 0x36

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

Servo servo1;

int pos = 0;
int speed = 0;
unsigned long lastEncoderReadTime = 0;
const unsigned long encoderReadInterval = 50;  // 50ms마다 엔코더 읽기
int previousRawAngle = 0;
int numberOfTurns = 0;

// Function declarations
void setupAS5600();
float readAngle();

void setup() {
  Serial.begin(9600);
  myWire.begin();

  stepper1.setMaxSpeed(500);
  stepper1.setAcceleration(500);
  stepper1.setCurrentPosition(0);

  stepper2.setMaxSpeed(500);
  stepper2.setAcceleration(500);
  stepper2.setCurrentPosition(0);

  stepper3.setMaxSpeed(500);
  stepper3.setAcceleration(500);
  stepper3.setCurrentPosition(0);

  stepper4.setMaxSpeed(500);
  stepper4.setAcceleration(500);
  stepper4.setCurrentPosition(0);
}

void loop() {
  // 엔코더 값을 일정 주기로만 읽기
  if (millis() - lastEncoderReadTime >= encoderReadInterval) {
    lastEncoderReadTime = millis();
    float totalAngle = readAngle(); // Read the current total angle from AS5600
    Serial.print("Total Angle: ");
    Serial.print(totalAngle);
    Serial.println(" degrees");
    delay(1); // Delay for 1 millisecond
  }

  // 모터 제어
  stepper1.runSpeed();
  stepper2.runSpeed();
  stepper3.runSpeed();
  stepper4.runSpeed();

  // 시리얼 통신을 통한 모터 제어
  if (Serial.available()) {
    char c = Serial.read();

    switch (c) {
      case 'q':
        Serial.println("'q'key is pressed");
        stepper1.setSpeed(500);
        break;
      case 'a':
        Serial.println("'a'key is pressed");
        stepper1.setSpeed(-500);
        break;
      case 'w':
        Serial.println("'w'key is pressed");
        stepper2.setSpeed(-1500);
        break;
      case 's':
        Serial.println("'s'key is pressed");
        stepper2.setSpeed(1500);
        break;
      case 'e':
        Serial.println("'e'key is pressed");
        stepper3.setSpeed(-500);
        break;
      case 'd':
        Serial.println("'d'key is pressed");
        stepper3.setSpeed(500);
        break;
      case 'r':
        Serial.println("'r'key is pressed");
        stepper4.setSpeed(500);
        break;
      case 'f':
        Serial.println("'f'key is pressed");
        stepper4.setSpeed(-500);
        break;
      case '1':
        Serial.println("'1' key is pressed");
        stepper1.setSpeed(0);
        stepper2.setSpeed(0);
        stepper3.setSpeed(0);
        stepper4.setSpeed(0);
        break;
      case 't':
        Serial.println("'t' key is pressed");
        servo1.write(0);
        break;
      case 'g':
        Serial.println("'g' key is pressed");
        servo1.write(90);
        break;
    }
  }
}

float readAngle() {
  myWire.beginTransmission(AS5600_ADDRESS);
  myWire.write(0x0C); // Register address for high byte of raw angle
  myWire.endTransmission();
  myWire.requestFrom(AS5600_ADDRESS, 2); // Request 2 bytes from AS5600

  if (myWire.available() == 2) {
    int highByte = myWire.read(); // Read high byte
    int lowByte = myWire.read(); // Read low byte

    int rawAngle = ((highByte << 8) | lowByte) & 0x0FFF; // Combine high and low bytes

    // Check if a full rotation has been made
    if (rawAngle - previousRawAngle > 2048) {
      numberOfTurns--;
    } else if (previousRawAngle - rawAngle > 2048) {
      numberOfTurns++;
    }

    previousRawAngle = rawAngle;

    // Calculate the total angle
    float totalAngle = (numberOfTurns * 360.0) + (rawAngle * (360.0 / 4096.0));
    return totalAngle;
  } else {
    // If data is not available, return an error value
    return -1.0;
  }
}