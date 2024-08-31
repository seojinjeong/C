#include <Wire.h>

// TCA9548A I2C address
#define TCA9548A_ADDRESS 0x70

// AS5600 I2C address
#define AS5600_ADDRESS 0x36

// Function declarations
void tcaSelect(uint8_t channel);
float readAngle();

void setup() {
  Serial.begin(9600); // Start the serial communication
  Wire.begin(); // Initialize I2C communication
  Wire.setClock(400000L); // Set I2C clock speed to 400kHz
}

void loop() {
  for (uint8_t i = 0; i < 4; i++) {
    tcaSelect(i); // Select the i-th channel on TCA9548A
    float totalAngle = readAngle(); // Read the current total angle from AS5600
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" Angle: ");
    Serial.print(totalAngle);
    Serial.println(" degrees");
  }
  delay(1000); // Delay for 1 second before the next reading
}

// Select a specific channel on the TCA9548A
void tcaSelect(uint8_t channel) {
  if (channel > 7) return;

  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

float readAngle() {
  Wire.beginTransmission(AS5600_ADDRESS);
  Wire.write(0x0C); // Register address for high byte of raw angle
  if (Wire.endTransmission() != 0) {
    return -1.0; // If transmission failed, return error
  }
  delay(1); // Small delay to ensure the sensor has time to process the request
  Wire.requestFrom(AS5600_ADDRESS, 2); // Request 2 bytes from AS5600

  if (Wire.available() == 2) {
    int highByte = Wire.read(); // Read high byte
    int lowByte = Wire.read(); // Read low byte

    int rawAngle = ((highByte << 8) | lowByte) & 0x0FFF; // Combine high and low bytes

    // Calculate the angle
    float angle = rawAngle * (360.0 / 4096.0);
    return angle;
  } else {
    // If data is not available, return an error value
    return -1.0;
  }
}

