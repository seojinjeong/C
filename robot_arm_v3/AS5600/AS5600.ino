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
}

void loop() {
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
