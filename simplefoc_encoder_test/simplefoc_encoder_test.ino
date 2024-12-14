#include <Wire.h>
#include <SimpleFOC.h>

// AS5600 센서 설정
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

// 센서 초기 오프셋 값
float angle_offset = 0.0;

void setup() {
  // I2C 통신 초기화
  Wire.begin();
  Wire.setClock(400000);  // 400kHz I2C 고속 모드
  Serial.begin(115200);

  // 센서 초기화
  sensor.init();
  Serial.println("AS5600 Initialized!");

  // 초기 오프셋 값 설정
  // 주의: 초기 오프셋을 설정하기 전에 sensor.update()를 먼저 한 번 실행하는 것도 방법
  sensor.update();
  angle_offset = sensor.getAngle();
  Serial.print("Initial Offset: ");
  Serial.println(angle_offset);
}

void loop() {
  // 매 루프마다 센서 값을 갱신
  sensor.update();

  // 센서 값 읽기 (라디안 단위 각도)
  float raw_angle = sensor.getAngle();
  float corrected_angle = raw_angle - angle_offset;  // 오프셋 보정된 각도
  float velocity = sensor.getVelocity();

  // 각도 출력

  Serial.print(corrected_angle);
  Serial.print(" ");
  Serial.println(velocity);

  delay(10);  // 100ms 주기
}
