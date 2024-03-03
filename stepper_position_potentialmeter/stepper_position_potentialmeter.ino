#include <AccelStepper.h>

#define pul 6
#define dir 7

int position;

//Global Variables
    int sensorPin = 0;      //pin number to use the ADC
    int sensorValue = 0;    //initialization of sensor variable, equivalent to EMA Y
    float EMA_a = 0.6;      //initialization of EMA alpha
    int EMA_S = 0;          //initialization of EMA S
 

AccelStepper stepper = AccelStepper(1, pul, dir);

void setup() {
  stepper.setMaxSpeed(500);
  EMA_S = analogRead(sensorPin);  //set EMA S for t=1
  Serial.begin(9600);
}

void loop() {
  sensorValue = map(analogRead(A0), 0, 1023, 0, 399);
  EMA_S = (EMA_a*sensorValue) + ((1-EMA_a)*EMA_S);
  stepper.setSpeed(2000);
  stepper.moveTo(EMA_S);
  stepper.runSpeedToPosition();
  Serial.println(EMA_S);
}