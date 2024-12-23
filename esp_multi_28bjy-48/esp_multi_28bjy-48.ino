#include <AccelStepper.h>

// 모터별 핀 할당 (각 모터에 대해 IN1, IN2, IN3, IN4)
#define M1_IN1 21
#define M1_IN2 22
#define M1_IN3 23
#define M1_IN4 25

#define M2_IN1 16
#define M2_IN2 17
#define M2_IN3 18
#define M2_IN4 19

#define M3_IN1 4
#define M3_IN2 5
#define M3_IN3 13
#define M3_IN4 14

#define M4_IN1 26
#define M4_IN2 27
#define M4_IN3 32
#define M4_IN4 33

// 각 모터 객체 생성
// (FULL4WIRE 모드, 기존 코드와 동일하게 핀 순서: IN1, IN3, IN2, IN4)
AccelStepper stepper1(AccelStepper::FULL4WIRE, M1_IN1, M1_IN3, M1_IN2, M1_IN4);
AccelStepper stepper2(AccelStepper::FULL4WIRE, M2_IN1, M2_IN3, M2_IN2, M2_IN4);
AccelStepper stepper3(AccelStepper::FULL4WIRE, M3_IN1, M3_IN3, M3_IN2, M3_IN4);
AccelStepper stepper4(AccelStepper::FULL4WIRE, M4_IN1, M4_IN3, M4_IN2, M4_IN4);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting 4 Stepper Motors...");

  // 각 모터 속도, 가속도 설정
  stepper1.setMaxSpeed(2000);
  stepper1.setAcceleration(500);
  stepper1.setCurrentPosition(0);

  stepper2.setMaxSpeed(2000);
  stepper2.setAcceleration(500);
  stepper2.setCurrentPosition(0);

  stepper3.setMaxSpeed(2000);
  stepper3.setAcceleration(500);
  stepper3.setCurrentPosition(0);

  stepper4.setMaxSpeed(2000);
  stepper4.setAcceleration(500);
  stepper4.setCurrentPosition(0);

  Serial.println("All steppers initialized.");
}

// 모든 스텝모터를 동시에 목표 위치까지 이동시키는 함수
void runAllSteppers() {
  while ((stepper1.distanceToGo() != 0) ||
         (stepper2.distanceToGo() != 0) ||
         (stepper3.distanceToGo() != 0) ||
         (stepper4.distanceToGo() != 0)) {
    stepper1.run();
    stepper2.run();
    stepper3.run();
    stepper4.run();
  }
}

void loop() {
  // 예시: 시계 방향으로 1회전 (512 스텝 가정, 필요 시 조정)
  Serial.println("Clockwise Rotation...");
  stepper1.moveTo(512 * 4);
  stepper2.moveTo(512 * 4);
  stepper3.moveTo(512 * 4);
  stepper4.moveTo(512 * 4);

  runAllSteppers(); // 모든 모터를 목표 지점까지 이동

  delay(1000);

  // 반시계 방향으로 돌아가기 (0 위치로 복귀)
  Serial.println("Counterclockwise Rotation...");
  stepper1.moveTo(0);
  stepper2.moveTo(0);
  stepper3.moveTo(0);
  stepper4.moveTo(0);

  runAllSteppers(); // 다시 0 위치로 이동

  delay(1000);
}
