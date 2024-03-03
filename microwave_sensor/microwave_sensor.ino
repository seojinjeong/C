 #include <Servo.h>
 
Servo myservo;
int increase = 4;
int min_angle = 15;
int max_angle = 130;
int pos = min_angle;
 
int TrigPin = 11;
int EchoPin = 10;

int a1 = 5;
int a2 = 6;
 
void setup(){
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
}
 
void loop(){
  myservo.write(pos);
  Serial.print("Position : ");
  Serial.print(pos);
  Serial.print(",");
  Serial.print("Distance : ");
  Serial.print(distance());
  Serial.print("\n");

  if(distance() < 15) {
    Serial.print("\n####### Warning #######\n");
    digitalWrite(a1, HIGH);
    digitalWrite(a2, LOW);
    delay(500);
  }

  else if(distance() >= 15) {
    digitalWrite(a1, HIGH);
    digitalWrite(a2, HIGH);
    delay(50);
  }
 
  pos += increase;
  if(pos <= min_angle || pos >= max_angle)
    increase *= -1;
 
  delay(30);
}
 
unsigned long distance(){
  unsigned long d;
   
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
   
  d = pulseIn(EchoPin, HIGH)*0.034/2;
   
  return d;
}
