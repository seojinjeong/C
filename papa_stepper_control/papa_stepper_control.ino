#include <AccelStepper.h>

#define DIR 3
#define STEP 4
#define RIGHT 6
#define LEFT 7

int val = 0;

AccelStepper stepper = AccelStepper(1, STEP, DIR);//드라이버를 쓸때는 1을쓴다!

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//결과를 시리얼모니터에서 볼거야~
  pinMode(DIR,OUTPUT);
  pinMode(STEP,OUTPUT);
  stepper.setMaxSpeed(1000); //얘는 200이 적절한값!
  stepper.setAcceleration(1500); //50~300사이 정도로 적당한값 찾기!
  stepper.setCurrentPosition(0); //현재 스탭을 설정
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);

}

void loop() {
  int button_state_RIGHT = digitalRead(RIGHT);
  int button_state_LEFT = digitalRead(LEFT);
  // Serial.println(button_state);

  int readValue = analogRead(A0);
  val = map(readValue, 0, 1023, 0, 1000);
  Serial.println(val);

  if (button_state_RIGHT == 0) {
    stepper.setSpeed(val*2);
    stepper.runSpeed();
  }

  else if (button_state_RIGHT == 1) {
    stepper.setSpeed(0);
    stepper.runSpeed();
  }

  if (button_state_LEFT == 0) {
    stepper.setSpeed(-val*2);
    stepper.runSpeed();
  }

  else if (button_state_LEFT == 1) {
    stepper.setSpeed(0);
    stepper.runSpeed();
  }
}