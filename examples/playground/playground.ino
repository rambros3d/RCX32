#include "RCX32.h"

RCX_Motors motor;

bool test;

void setup() {
  Serial.begin(115200);
  delay(200);
  motor.add2pwmDriver(MOTOR1, 2, 48);
  //motor.add2pwmDriver(MOTOR1, 14, 15,11);
  motor.init();
  Serial.print("code overflow ");
  Serial.println(test);
}

void loop() {
  motor.run(MOTOR1, 60, FORWARD);
  delay(250);
  motor.run(MOTOR1, 100, BACKWARD);
  delay(1000);
  delay(1);  // added for stability
}