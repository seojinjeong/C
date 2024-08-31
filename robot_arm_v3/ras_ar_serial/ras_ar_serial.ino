#include <AccelStepper.h>

#define DIR 3
#define STEP 4
char cmd;

AccelStepper stepper = AccelStepper(1, STEP, DIR);//드라이버를 쓸때는 1을쓴다!

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//결과를 시리얼모니터에서 볼거야~
  pinMode(DIR,OUTPUT);
  pinMode(STEP,OUTPUT);
  stepper.setMaxSpeed(1000); //얘는 200이 적절한값!
}

void loop() {
  
  if(Serial.available()) {
    cmd = Serial.read(" : ");
    // cmd.trim();
  }

  if(cmd == 'a') {
    stepper.setSpeed(500);
    stepper.moveTo(400);
    stepper.runToPosition();
  }

  else if(cmd == 'b') {
    stepper.setSpeed(500);
    stepper.moveTo(-400);
    stepper.runToPosition();
  }
}