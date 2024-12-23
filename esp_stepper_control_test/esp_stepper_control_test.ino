#include <AccelStepper.h>

// ULN2003 드라이버 보드 핀 정의
// #define IN1 21
// #define IN2 22
// #define IN3 23
// #define IN4 25

// #define IN1 16
// #define IN2 17
// #define IN3 18
// #define IN4 19

// #define IN1 4
// #define IN2 5
// #define IN3 13
// #define IN4 14

#define IN1 26
#define IN2 27
#define IN3 32
#define IN4 33


// AccelStepper 객체 생성 (모터 유형: 4핀 모터)
AccelStepper stepper(AccelStepper::FULL4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Stepper Motor...");

  // 모터 속도 및 가속도 설정
  stepper.setMaxSpeed(2000);      // 최대 속도 (단위: 스텝/초)
  stepper.setAcceleration(500);  // 가속도 (단위: 스텝/초^2)

  // 초기 위치 지정
  stepper.setCurrentPosition(0); // 초기 위치를 0으로 설정

    // 각 모터 속도 설정 (양수값 -> 한 방향 회전, 음수값 -> 반대 방향 회전)
  // stepper.setSpeed(500);
}

void loop() {
  // 시계 방향으로 1회전 (512 스텝)
  Serial.println("Clockwise Rotation...");
  stepper.moveTo(512 * 4); // 목표 위치 설정
  while (stepper.distanceToGo() != 0) {
    stepper.run(); // 모터 실행 (가속/감속 포함)
  }
  delay(1000); // 대기

  // 반시계 방향으로 1회전 (512 스텝)
  Serial.println("Counterclockwise Rotation...");
  stepper.moveTo(0); // 다시 초기 위치로 이동
  while (stepper.distanceToGo() != 0) {
    stepper.run(); // 모터 실행 (가속/감속 포함)
  }
  // delay(1000); // 대기
  // stepper.runSpeed();
}
