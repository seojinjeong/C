#include <AccelStepper.h>
#include <Servo.h>

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

#include <Wire.h>

// AS5600 I2C address
#define AS5600_ADDRESS 0x36

// Global variables
int previousRawAngle = 0;
int numberOfTurns = 0;

// Function declarations
void setupAS5600();
float readAngle();

void setup() {
  Serial.begin(115200); // Start the serial communication
  Wire.begin(); // Initialize I2C communication
  Wire.setClock(400000L); // Set I2C clock speed to 400kHz

  setupAS5600(); // Initialize AS5600

  // pinMode(step1, OUTPUT);
  // pinMode(step2, OUTPUT);
  // pinMode(step3, OUTPUT);
  // pinMode(step4, OUTPUT);
  // pinMode(dir1, OUTPUT);
  // pinMode(dir2, OUTPUT);
  // pinMode(dir3, OUTPUT);
  // pinMode(dir4, OUTPUT);

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
  // digitalWrite(dir1, HIGH);
  // digitalWrite(dir2, HIGH);
  // digitalWrite(dir3, HIGH);

  // for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   servo1.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   servo1.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // servo1.write(0);

  // 0 degree : closed , 90 degree : open

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
      stepper1.setSpeed(500);
    }

    else if(c == 'a') {
      Serial.println("'a'key is pressed");
      stepper1.setSpeed(-500);
    }

    else if(c == 'w') {
      Serial.println("'w'key is pressed");
      stepper2.setSpeed(-1500);
    }

    else if(c == 's') {
      Serial.println("'s'key is pressed");
      stepper2.setSpeed(1500);
    }

    else if(c == 'e') {
      Serial.println("'e'key is pressed");
      stepper3.setSpeed(-500);
    }

    else if(c == 'd') {
      Serial.println("'d'key is pressed");
      stepper3.setSpeed(500);
    }

    else if(c == 'r') {
      Serial.println("'r'key is pressed");
      stepper4.setSpeed(500);
    }

    else if(c == 'f') {
      Serial.println("'f'key is pressed");
      stepper4.setSpeed(-500);
    }

    else if(c == '1') {
      Serial.println("'1' key is pressed");
      stepper1.setSpeed(0);
      stepper2.setSpeed(0);
      stepper3.setSpeed(0);
      stepper4.setSpeed(0);
      // stepper1.runSpeed();
      // delay(500);
    }

    else if (c == 't') {
      Serial.println("'t' key is pressed");
      servo1.write(0);
    }

    else if (c == 'g') {
      Serial.println("'g' key is pressed");
      servo1.write(90);
    }
  }
  stepper1.runSpeed(); 
  stepper2.runSpeed();
  stepper3.runSpeed();
  stepper4.runSpeed();

  float totalAngle = readAngle(); // Read the current total angle from AS5600
  Serial.print("Total Angle: ");
  Serial.print(totalAngle);
  Serial.println(" degrees");
  delay(1); // Delay for 1 millisecond
}

void setupAS5600() {
  // Nothing specific to setup for AS5600 in this simple example
}

float readAngle() {
  Wire.beginTransmission(AS5600_ADDRESS);
  Wire.write(0x0C); // Register address for high byte of raw angle
  Wire.endTransmission();
  Wire.requestFrom(AS5600_ADDRESS, 2); // Request 2 bytes from AS5600

  if (Wire.available() == 2) {
    int highByte = Wire.read(); // Read high byte
    int lowByte = Wire.read(); // Read low byte

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

