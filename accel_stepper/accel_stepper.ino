/*
AccelStepper stepper = AccelStepper(연결방식, STEP핀, DIR핀);
stepper.setMaxSpeed(1000); //최대속도
stepper.setCurrentPosition(0); //현재 스탭을 설정
stepper.currentPosition(); //현재 스탭을 반환
stepper.setSpeed(200); //스탭모터의 방향과 속도를 설정
stepper.runSpeed(); //스탭모터 작동(고정속도)
stepper.setAcceleration(2000); //가속량을 설정함
stepper.moveTo(1000); //목표스탭량을 설정함
stepper.runToPosition(); //가속도를 붙혀서 스탭모터제어
*/

#include <AccelStepper.h>

#define DIR 3
#define STEP 4

AccelStepper stepper = AccelStepper(1, STEP, DIR);//드라이버를 쓸때는 1을쓴다!

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//결과를 시리얼모니터에서 볼거야~
  pinMode(DIR,OUTPUT);
  pinMode(STEP,OUTPUT);
  stepper.setMaxSpeed(1000); //얘는 200이 적절한값!
  stepper.setAcceleration(1500); //50~300사이 정도로 적당한값 찾기!
  stepper.setCurrentPosition(0); //현재 스탭을 설정
}

void loop() {
  // put your main code here, to run repeatedly:
  //시계방향으로 10바퀴 회전
  stepper.setSpeed(1000);
  Serial.println("시계방향으로 10바퀴 회전 시작!");
  stepper.moveTo(8000); //목표스탭량을 설정함 --> 10cycle ( 200step -> 1cycle )
  stepper.runToPosition(); //가속도를 붙혀서 스탭모터제어
  Serial.println("시계방향으로 10바퀴 회전 종료!");
  delay(2000);
  //반시계방향으로 10바퀴 회전
  Serial.println("반시계방향으로 10바퀴 회전 시작!");
  stepper.moveTo(0); //목표스탭량을 설정함
  stepper.runToPosition(); //가속도를 붙혀서 스탭모터제어
  Serial.println("반시계방향으로 10바퀴 회전 종료!");
  delay(2000);
}
