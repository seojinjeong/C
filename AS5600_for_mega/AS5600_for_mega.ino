#include <SoftwareWire.h>

// SCL과 SDA를 다른 핀에 할당
SoftwareWire myWire(A8, A9);  // SDA = A8, SCL = A9

#define AS5600_ADDRESS 0x36

void setup() {
  Serial.begin(115200);
  myWire.begin();
}

void loop() {
  myWire.beginTransmission(AS5600_ADDRESS);
  myWire.write(0x0C); // raw angle의 상위 바이트 주소
  myWire.endTransmission();
  myWire.requestFrom(AS5600_ADDRESS, 2);

  if (myWire.available() == 2) {
    int highByte = myWire.read();
    int lowByte = myWire.read();

    int rawAngle = ((highByte << 8) | lowByte) & 0x0FFF;

    float angle = rawAngle * (360.0 / 4096.0);
    Serial.print("Angle: ");
    Serial.println(angle);
  }

  delay(100);
}
