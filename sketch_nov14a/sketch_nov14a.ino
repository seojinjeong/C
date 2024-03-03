// Define pin connections & motor's steps per revolution
const int dirPin1 = 2;
const int stepPin1 = 3;
const int dirPin2 = 5;
const int stepPin2 = 6;
const int stepsPerRevolution = 1200; //revolve 6times because the one cycle is 200
const int cycle_max = 2000;
const int cycle_min = 400;

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
}
void loop()
{
  // Set motor direction clockwise
   digitalWrite(dirPin1, HIGH);
   digitalWrite(dirPin2, HIGH);

  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(800);
  }
  delay(1000);
  // Set motor direction clockwise
  digitalWrite(dirPin2, HIGH);

  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(800);
  }
  delay(1000);
}
