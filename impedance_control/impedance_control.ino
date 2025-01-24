#include <SimpleFOC.h>
#include <math.h>

// 1) 센서 설정: AS5600 I2C
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);

// 2) BLDC 모터 & 드라이버 설정
//    - 모터: BLDCMotor(pole_pairs) -> 5010 모터라면 극쌍수(pole_pairs) 7 등
BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(9, 5, 6, 8);

// 임피던스 제어 파라미터
float K_spring = 1.0;   // 스프링 상수
float C_damper = 0.5;   // 감쇠 계수
float ref_angle = 0.0;  // 평형 각도 (rad)

// Commander 인스턴스
Commander command = Commander(Serial);

// 커맨드 파서 함수들
void doRefAngle(char* cmd) { command.scalar(&ref_angle, cmd); }
void doKspring(char* cmd) { command.scalar(&K_spring, cmd); }
void doCdamper(char* cmd) { command.scalar(&C_damper, cmd); }

void setup() {
  Serial.begin(115200);
  while(!Serial);

  // 센서 초기화
  sensor.init();
  // 모터에 센서 연결
  motor.linkSensor(&sensor);

  // 드라이버 초기화
  driver.voltage_power_supply = 24;  // 전원 전압 (예: 12V)
  driver.init();
  motor.linkDriver(&driver);

  // FOC 모듈레이션
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

  // *** 토크 제어 모드로 설정 ***
  motor.controller = MotionControlType::torque;

  // 전압 제한 (토크 제어 시 전압 제한이 곧 토크 제한에 영향)
  motor.voltage_limit = 10; // 예시값

  // PID 설정 (내부 속도 루프/전류 루프용)
  motor.PID_velocity.P = 0.1f;
  motor.PID_velocity.I = 1.7f;
  motor.PID_velocity.D = 0;
  motor.LPF_velocity.Tf = 0.01;

  // 속도 제한 (rad/s)
  motor.velocity_limit = 10;

  // 모니터링 (Serial Plotter)
  motor.useMonitoring(Serial);
  motor.monitor_variables = _MON_VEL | _MON_ANGLE | _MON_VOLT_Q;
  motor.monitor_downsample = 10;

  // 모터 초기화 + 센서 얼라인
  motor.init();
  motor.initFOC();

  // Commander 명령 추가
  command.add('R', doRefAngle, "Reference angle [rad]");
  command.add('K', doKspring,  "K_spring");
  command.add('C', doCdamper,  "C_damper");

  Serial.println("Impedance Control (Spring+Damper) Ready.");
  Serial.println("Commands:");
  Serial.println("  R <val> : set reference angle (rad)");
  Serial.println("  K <val> : set spring constant");
  Serial.println("  C <val> : set damping constant");

  _delay(1000);
}

void loop() {
  // 1) FOC 실행 (전류 루프 등 내부 제어)
  motor.loopFOC();

  // 2) 임피던스 제어 식 (토크 계산)
  float theta = motor.shaft_angle;
  float dtheta = motor.shaft_velocity;

  // 스프링 + 댐퍼 토크
  float torque_cmd = - K_spring * (theta - ref_angle)
                     + C_damper * dtheta;

  // 3) 토크 명령 설정 (토크 제어 모드)
  motor.move(torque_cmd);

  // 4) 모니터링
  motor.monitor();

  // 5) 시리얼 명령 파싱
  command.run();
}
