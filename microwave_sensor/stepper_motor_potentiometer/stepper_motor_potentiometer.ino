int steppin = 4;
int dirpin = 3;
int step_delay;

void setup() {
  pinMode(steppin, OUTPUT);
  pinMode(dirpin, OUTPUT);
  digitalWrite(dirpin, HIGH);


}

void loop() {
  int val = analogRead(A0);
  step_delay = map(val, 0, 1024, 1, 1000);
  digitalWrite(steppin, HIGH);
  delayMicroseconds(step_delay);
  digitalWrite(steppin, LOW);
  delayMicroseconds(step_delay);

}
