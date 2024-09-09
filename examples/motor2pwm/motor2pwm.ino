#include <RCX32.h>
#define driverEnablePin 11

// Define motor pins
const uint8_t motorPin1 = 12;
const uint8_t motorPin2 = 13;
// Define motor frequency and resolution
const uint16_t pwmFrequency = 24000;
const uint8_t pwmResolution = 10;

Motor2pwm motor(motorPin1, motorPin2, pwmFrequency, pwmResolution);

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(driverEnablePin, OUTPUT);
  digitalWrite(driverEnablePin, !HIGH);
  motor.init();
}

void loop() {
  // Set motor speed to 50% and direction to forward
  motor.set(25, true);
  Serial.print("Motor Speed: ");
  Serial.print(motor.getSpeed());
  Serial.print(", Direction: ");
  Serial.println(motor.getDirection() ? "Forward" : "Backward");
  delay(1000);

  // Set motor speed to 25% and direction to backward
  motor.set(30, false);
  Serial.print("Motor Speed: ");
  Serial.print(motor.getSpeed());
  Serial.print(", Direction: ");
  Serial.println(motor.getDirection() ? "Forward" : "Backward");
  delay(1000);

  // Stop the motor
  motor.stop();
  Serial.print("Motor Speed: ");
  Serial.print(motor.getSpeed());
  Serial.print(", Direction: ");
  Serial.println(motor.getDirection() ? "Forward" : "Backward");
  delay(1000);
}