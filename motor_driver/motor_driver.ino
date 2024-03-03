int a1 = 5;
int a2 = 6;

void setup() {
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
}

void loop() {
  digitalWrite(a1, HIGH);
  digitalWrite(a2, LOW);
  delay(2000);
  digitalWrite(a1, LOW);
  digitalWrite(a2, HIGH);
  delay(2000);
  digitalWrite(a1, HIGH);
  digitalWrite(a2, HIGH);
  delay(2000);
}
