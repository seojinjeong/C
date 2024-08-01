#include <AccelStepper.h>

#define DIR 3
#define STEP 4

String serial_data = "";

AccelStepper stepper = AccelStepper(1, STEP, DIR);//드라이버를 쓸때는 1을쓴다!

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//결과를 시리얼모니터에서 볼거야~
  pinMode(DIR,OUTPUT);
  pinMode(STEP,OUTPUT);
  stepper.setMaxSpeed(1000); //얘는 200이 적절한값!
  stepper.setAcceleration(1000);
}

void loop() {
  while(Serial.available()) {
    serial_data = Serial.read();
  }

  if(serial_data == '1') {
    Serial.println("Hello");
    stepper.setSpeed(1000);
    stepper.moveTo(1000);
    stepper.runToPosition();
  }
}