#include <math.h>


float theta1(int x, int y) {
  int L1 = 162;
  int L2 = 132;
  // float alpha = acos((square(L1) + square(x) + square(y) - square(L2))/(2 * L1 * sqrt(square(x) + square(y))));
  float alpha = acos((square(x) + square(y) + square(L1) - square(L2)) / (2 * L1 * sqrt(square(x) + square(y))));
  float theta3 = atan(y/x) + alpha;
  return degrees(theta3);
}   

  float theta2(float x, float y) {
  float L1 = 162;
  float L2 = 132;
  // float beta = acos((square(L1) + square(L2) -square(x) -square(y))/(2 * L1 * L2));
  float beta = acos((square(L1) + square(L2) -square(x) -square(y))/(2 * L1 * L2));
  float theta4 = 3.14-beta;
  return degrees(theta4); 
}



void setup() {
  Serial.begin(9600);
  float L1 = 162;
  float L2 = 132;
  float x = 100;
  float y = 0;
  float alpha = acos((square(x) + square(y) + square(L1) - square(L2)) / (2 * L1 * sqrt(square(x) + square(y))));
  float beta = acos((square(L1) + square(L2) -square(x) -square(y))/(2 * L1 * L2));
  Serial.println("===================================");
  Serial.println(alpha);
  Serial.println(beta);
  Serial.println("===================================");

  Serial.println("===================================");
  Serial.println(theta1(100, 0));
  Serial.println(theta2(100, 0));
  Serial.println(degrees(0.95));
  Serial.println(degrees(0.66));
  Serial.println("===================================");

}


void loop() {
  // put your main code here, to run repeatedly:

}
