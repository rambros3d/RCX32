#include "RCX32.h"

RCX_Motors motor;

void setup() {
  Serial.begin(115200);
  delay(2000);
  motor.add1pwmDriver(MOTOR1, 2, -48);
  delay(500);
  motor.init();
}

void loop() {
  motor.setSpeed(MOTOR1, 50, true);
  delay(500);
  motor.setSpeed(MOTOR1, 100, false);
  delay(1000);
  delay(1);  // added for stability
}