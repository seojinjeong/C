/**
 * Closed-loop velocity control with AS5600 & SimpleFOC
 *
 * 1) 센서: MagneticSensorI2C (AS5600)
 * 2) 모터 & 드라이버: BLDCMotor + BLDCDriver3PWM
 * 3) 속도 제어(FOC) 모드
 * 4) Commander(시리얼 명령어)로 T, L 변경 가능
 */

#include <Arduino.h>
#include <Wire.h>
#include <SimpleFOC.h>


//////////////////////
// 모터/드라이버/센서
//////////////////////

// 1) BLDC 모터: 폴페어 수를 실제 모터에 맞춰 설정
BLDCMotor motor = BLDCMotor(7);  

// 2) BLDC 드라이버 (3상 PWM + Enable 핀 옵션)
//    예: Arduino UNO에서 pwmA=9, pwmB=5, pwmC=6, enablePin=8
BLDCDriver3PWM driver = BLDCDriver3PWM(9, 5, 6, 8);

// 3) AS5600 I2C 센서
//    AS5600_I2C는 SimpleFOC에 정의된 상수(0x36 주소 등)를 포함
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

float angle_offset = 0.0;


//////////////////////
// 초기화 (setup)
//////////////////////
void setup() {
  // 시리얼 모니터 초기화
  Serial.begin(115200);
  delay(500);

  Wire.begin();
  Wire.setClock(400000);  // 400kHz I2C 고속 모드

  // 센서 초기화
  sensor.init();
  Serial.println("AS5600 Initialized!");

  // 초기 오프셋 값 설정
  // 주의: 초기 오프셋을 설정하기 전에 sensor.update()를 먼저 한 번 실행하는 것도 방법
  sensor.update();
  angle_offset = sensor.getAngle();
  Serial.print("Initial Offset: ");
  Serial.println(angle_offset);

  // 2) 드라이버 설정
  driver.voltage_power_supply = 12;  // 예: 12V 전원
  driver.init();
  Serial.println("Driver initialized!");

  // 3) 모터와 드라이버/센서 연결
  motor.linkDriver(&driver);
  motor.linkSensor(&sensor);

  // 4) 모터 기본 설정
  // 최대 인가 전압 (예: 3~6V로 낮게 시작해 발열 점검)
  motor.voltage_limit = 3.0;
  // 속도 제한
  motor.velocity_limit = 10.0;
  // 제어 모드: 속도 폐루프
  // motor.controller = MotionControlType::velocity;
  motor.controller = MotionControlType::velocity_openloop;

  motor.voltage_limit = 3; // 낮은 전압에서 시작
  // 5) 모터 초기화
  motor.init();
  Serial.println("Motor init complete!");

  delay(1000);
}


//////////////////////
// 메인 루프 (loop)
//////////////////////
void loop() {

  motor.move(-20);  // 오픈 루프 5 rad/s


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
