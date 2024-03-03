#include <math.h>

void setup() {
  Serial.begin(9600);
  int L1 = 3;
  int L2 = 2;
  int x = 4;
  int y = 2;
  float alpha = acos((square(L1) + square(x) + square(y))/(2 * L1 * sqrt(square(x) + square(y))));
  float beta = acos((square(L1) + square(L2) -square(x) -square(y))/(2 * L1 * L2));
  Serial.println(alpha);
  Serial.println(beta);
  Serial.println(sqrt(2));
  Serial.println((square(L1) + square(x) + square(y) - square(L2))/(2 * L1 * sqrt(square(x) + square(y))));
}

void loop() {
  // put your main code here, to run repeatedly:

}
