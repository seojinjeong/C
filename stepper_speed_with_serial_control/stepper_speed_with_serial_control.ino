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

#define DIR 2
#define STEP 3

AccelStepper stepper = AccelStepper(1, STEP, DIR);//드라이버를 쓸때는 1을쓴다!

int speed = 0; // 초당 200스텝(초당 360도)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//결과를 시리얼모니터에서 볼거야~
  pinMode(DIR,OUTPUT);
  pinMode(STEP,OUTPUT);
  stepper.setMaxSpeed(1000); //초당 몇스탭이냐? 1000/s(초당5회전)
  stepper.setCurrentPosition(0); //현재 스탭을 설정
}

//메인스레드
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    char c = Serial.read();
    if(c == '+'){
      speed += 200;
      if(speed > 1000) speed = 1000;

      Serial.print("속도가 증가하였습니다!! 현재속도=");
      Serial.println(speed);
    }else if(c == '-'){
      speed -= 200;
      if(speed < -1000) speed = -1000;

      Serial.print("속도가 감소하였습니다!! 현재속도=");
      Serial.println(speed);
    }else if(c == '!'){
      speed = 0;

      Serial.println("브레이크!!!!!");
    }
    //변경된 속도를 반영한다!
    stepper.setSpeed(speed);
  }
  //스탭모터야 돌아라~~
  stepper.runSpeed();
}