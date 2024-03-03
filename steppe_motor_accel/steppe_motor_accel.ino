// Define pin connections & motor's steps per revolution
const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 1200; //revolve 6times because the one cycle is 200
const int cycle_max = 2000;
const int cycle_min = 400;

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}
void loop()
{
  // Set motor direction clockwise
  digitalWrite(dirPin, HIGH);

  // Spin motor slowly
//  for(int x = 0; x < stepsPerRevolution; x++)
//  {
  for(int x=2000; x > cycle_min; x += -3){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(x);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(x);  
  }

  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(400);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(400);
  }

  for(int x = 400; x < cycle_max; x += 3){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(x);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(x);  
  }
}
