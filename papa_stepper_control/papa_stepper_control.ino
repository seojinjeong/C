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
  pinMode(7, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //시계방향으로 10바퀴 회전
  int button_state = digitalRead(7);
  Serial.println(button_state);

  if (button_state == HIGH) {
    stepper.setSpeed(400);
    stepper.runSpeed();
  }

  else if (button_state == LOW) {
    stepper.setSpeed(0);
    stepper.runSpeed();
  }
}