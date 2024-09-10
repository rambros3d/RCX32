#include "RCX32.h"

RCX_Motors motor;

bool test;

void setup() {
  Serial.begin(115200);
  delay(200);
  // motor.add2pwmDriver(MOTOR1, 2, 48);
  motor.add2pwmDriver(MOTOR1, 12, 13, 11);
  motor.init();
}

void loop() {
  motor.setSpeed(MOTOR1, 40, FORWARD);
  motor.update(MOTOR1);
  delay(1000);
  // motor.setSpeed(MOTOR1, 100, BACKWARD);
  // motor.update(MOTOR1);
  // motor.run(MOTOR1, 80, REVERSE);
  motor.setSpeed(MOTOR1, 60, REVERSE);
  motor.update(MOTOR1);
  delay(500);
  delay(1); // added for stability
}