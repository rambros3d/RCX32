/*
  RCX32 Library - Arduino ESP32
  Rambros Workshop (@rambrosteam)
  https://github.com/rambrosteam/RCX32
  ----------------------------------------------------------------------------

  Serial console - Motor Control

  This example uses the RCX32 library to control a motor using serial commands.
  The motor driver is connected to pin 12, 13 and 11 of the ESP32 board (change
  pins as required). The motor speed can be set by sending a number between 0
  and 100 through the serial terminal. The motor direction can be set by sending
  a number with a negative sign. The motor can be stopped by sending 0 to the
  serial terminal.
*/

#include "RCX32.h"

RCX_Motors motor;
// Change the pin numbers if your motor is connected to different pins.
const int8_t pin1 = 13; // Pwm pin 1
const int8_t pin2 = 12; // Pwm pin 2 or Direction pin
const int8_t pin3 = 11; // Enable pin (optinal)

int16_t input = 0;
int16_t speed = 0;
bool direction;

void setup() {
  // Start serial communication
  Serial.begin(115200);
  delay(100);

  // motor.add2pwmDriver(MOTOR1, pin1, pin2);       // 2 pwm driver
  motor.add2pwmDriver(MOTOR1, pin1, pin2, pin3); //  2 pwm driver with enable
  // motor.add1pwmDriver(MOTOR1, pin1, pin2);       // 1 pwm 1 direction driver
  // motor.add1pwmDriver(MOTOR1, pin1, pin2, pin3); // 1 pwm 1 direction with EN

  motor.init(); // Initialize all motors

  Serial.println("\n\nWelcome to the serial motor control example.");
  Serial.println("Type a number between -100 and 100 to set the motor speed");
  Serial.println("Negative numbers will reverse the motor direction\n");
}

void loop() {
  // Check if there is any serial input available
  if (Serial.available() > 0) {
    // Read the input string until the next line break
    String inputStr = Serial.readStringUntil('\n');
    input = inputStr.toInt();

    // convert input to absolute value
    speed = abs(input);

    // map input value (0-100) to pwm duty cycle (0-1023)
    speed = map(speed, 0, 100, 0, 1023);

    // constrain pwm duty cycle to valid range (10bit)
    speed = constrain(speed, 0, 1023);

    // set motor direction reverse if input is negative
    direction = (input > 0) ? FORWARD : REVERSE;

    Serial.print("Input: ");
    Serial.print(input);
    Serial.print("\t PWM Duty: ");
    Serial.print(speed);
    Serial.print("\tDirection: ");
    Serial.println(direction ? "Forward" : "Backward");

    // Set the motor speed and direction
    if (direction == FORWARD) {
      motor.run(MOTOR1, speed, FORWARD);
    } else {
      motor.run(MOTOR1, speed, REVERSE);
    }
  }
  delay(1); // added for stability
}
