#include <AccelStepper.h>

// 초음파 센서 핀 정의
#define TRIG_PIN 17
#define ECHO_PIN 5

// 스텝 모터 핀 정의
#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25

// 초음파 감지 거리 (cm)
const int DISTANCE_THRESHOLD = 10;

// 스텝 모터 객체 생성 (FULL4WIRE 방식)
AccelStepper stepper(AccelStepper::FULL4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);

  // 초음파 센서 핀 설정
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // 스텝 모터 초기 설정
  stepper.setMaxSpeed(1000);       // 최대 속도 (스텝/초)
  stepper.setAcceleration(500);   // 가속도 (스텝/초^2)
  stepper.setCurrentPosition(0);  // 현재 위치를 0으로 설정

  Serial.println("Ultrasonic sensor with stepper motor control initialized.");
}

void loop() {
  // 거리 측정
  long distance = measureDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 거리 임계값 확인
  if (distance > 0 && distance <= DISTANCE_THRESHOLD) {
    Serial.println("Object detected! Rotating stepper motor...");
    
    // 스텝 모터 90도 회전 (CW)
    stepper.moveTo(512);  // 90도 = 512 스텝 (28BYJ-48 기준)
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
    delay(2000);  // 2초 대기

    // 스텝 모터 원래 위치로 복귀 (CCW)
    stepper.moveTo(0);
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }

    Serial.println("Stepper motor returned to original position.");
  }

  delay(500);  // 0.5초 대기 후 거리 재측정
}

// 초음파 센서를 사용하여 거리 측정
long measureDistance() {
  // 초음파 펄스 전송
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // ECHO 핀의 펄스 길이 측정
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 타임아웃 30ms (약 5m)
  if (duration == 0) {
    return -1;  // 타임아웃 발생 시 -1 반환
  }

  // 거리 계산 (음속: 343 m/s)
  long distance = duration * 0.034 / 2;  // cm 단위 거리
  return distance;
}
