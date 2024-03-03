// Define pin connections & motor's steps per revolution
#define dirPin 2
#define stepPin 3

int customDelay, customDelayMapped;

void speedControl() {
  customDelay = analogRead(A0);
  customDelayMapped = map(customDelay, 0, 1023, 300,3000);
}

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}
void loop(){
  speedControl();
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(customDelayMapped);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(customDelayMapped);
}
