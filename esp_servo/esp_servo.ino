#include <ESP32Servo.h>

int servoPin = 14; // 서보 모터 핀
Servo myservo;     // 서보 모터 객체

void setup() {
   // 모든 타이머 할당 허용
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
   
  Serial.begin(115200);
  myservo.attach(servoPin); // 서보 모터를 핀에 연결
}

void loop() {
   // 서보 모터를 0도에서 90도 사이로 회전
   for (int angle = 0; angle <= 90; angle++) {
      myservo.write(angle);
      delay(20); // 각도 변경 후 대기 시간
   }
   delay(1000); // 회전 후 대기 시간
   
   // 서보 모터를 90도에서 0도 사이로 회전
   for (int angle = 90; angle >= 0; angle--) {
      myservo.write(angle);
      delay(20); // 각도 변경 후 대기 시간
   }
   delay(1000); // 회전 후 대기 시간
}
