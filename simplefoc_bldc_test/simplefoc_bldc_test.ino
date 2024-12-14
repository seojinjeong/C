#include <SimpleFOC.h>
#include <Wire.h>

// BLDC motor instance
BLDCMotor motor = BLDCMotor(11);  // 11은 모터 폴 수

// BLDC driver instance
BLDCDriver3PWM driver = BLDCDriver3PWM(3, 5, 6);  // Arduino UNO의 PWM 핀: D3, D5, D6

// Magnetic sensor instance (AS5600)
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

// Initialization
void setup() {
  // I2C 초기화
  Wire.begin();

  // Sensor 초기화
  sensor.init();
  Serial.begin(115200);
  Serial.println("AS5600 Sensor Initialized!");

  // 모터에 센서 연결
  motor.linkSensor(&sensor);

  // 모터 드라이버 설정
  driver.voltage_power_supply = 12;  // 전원 입력값
  driver.init();
  motor.linkDriver(&driver);

  // 제어 타입 설정
  motor.controller = MotionControlType::velocity;  // 속도 제어 모드

  // 전압 및 속도 제한
  motor.voltage_limit = 6;  // 최대 전압
  motor.velocity_limit = 10;  // 최대 속도 (rad/s)

  // PID 설정
  motor.PID_velocity.P = 0.2;
  motor.PID_velocity.I = 0.5;

  // 모터 초기화
  motor.init();
  Serial.println("Motor Initialized!");
}

void loop() {
  // FOC 루프
  motor.loopFOC();
  motor.move(2);  // 속도 명령 (2 rad/s)

  // 센서 각도 확인
  Serial.print("Sensor Angle: ");
  Serial.println(sensor.getAngle());
}
