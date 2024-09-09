// RCX_motor.cpp
#include "RCX_motor.h"


RCX_Motors::RCX_Motors() {
  motors = nullptr;
  numMotors = 0;
  pwmResoltuion = (1 << DefaultPwmResolution - 1);
}

void RCX_Motors::add2pwmDriver(MotorType type, int8_t pwmPin1, int8_t pwmPin2, int8_t enablePin) {
  Motor *newMotors = (Motor *)realloc(motors, (numMotors + 1) * sizeof(Motor));
  if (newMotors == nullptr) {
    return;  // Handle memory allocation error
  }
  motors = newMotors;
  motors[numMotors].type = type;

  motors[numMotors].invertPin1 = (pwmPin1 < 0) ? true : false;
  motors[numMotors].invertPin2 = (pwmPin2 < 0) ? true : false;
  motors[numMotors].pin1 = abs(pwmPin1);
  motors[numMotors].pin2 = abs(pwmPin2);

  if (enablePin == 0) {
    motors[numMotors].driverType = TWO_PWM_DRIVER;
  } else {
    motors[numMotors].invertEnable = (enablePin < 0) ? true : false;
    motors[numMotors].enPin = abs(enablePin);
    motors[numMotors].driverType = TWO_PWM_EN_DRIVER;
  }

  numMotors++;
}

void RCX_Motors::add1pwmDriver(MotorType type, int8_t pwmPin, int8_t dirPin, int8_t enablePin) {
  Motor *newMotors = (Motor *)realloc(motors, (numMotors + 1) * sizeof(Motor));
  if (newMotors == nullptr) {
    return;  // Handle memory allocation error
  }
  motors = newMotors;
  motors[numMotors].type = type;

  motors[numMotors].invertPin1 = (pwmPin < 0) ? true : false;
  motors[numMotors].invertPin2 = (dirPin < 0) ? true : false;
  motors[numMotors].pin1 = abs(pwmPin);
  if(dirPin != 0) motors[numMotors].pin2 = abs(dirPin);

  if (enablePin == 0) {
    motors[numMotors].driverType = ONE_PWM_DRIVER;
  } else {
    motors[numMotors].invertEnable = (enablePin < 0) ? true : false;
    motors[numMotors].enPin = abs(enablePin);
    motors[numMotors].driverType = ONE_PWM_EN_DRIVER;
  }

  numMotors++;
}

void RCX_Motors::setSpeed(MotorType type, int16_t inputSpeed, bool direction) {
  for (int i = 0; i < numMotors; i++) {
    if (motors[i].type == type) {
      ledcWrite(motors[i].pin1, direction ? pwmResoltuion : 0);
      ledcWrite(motors[i].pin2, direction ? 0 : pwmResoltuion);
    }
    break;
  }
}


void RCX_Motors::init(int16_t pwmFreq, int16_t pwmRes) {
  for (int i = 0; i < numMotors; i++) {
    switch (
      motors[i].driverType) {
      case TWO_PWM_DRIVER:
        // 2 pwm pins for speed and direction
        ledcAttach(motors[i].pin1, pwmFreq, pwmRes);
        ledcAttach(motors[i].pin2, pwmFreq, pwmRes);
        break;
      case TWO_PWM_EN_DRIVER:
        // 2 pwm pins for speed and direction and 1 enable pin
        ledcAttach(motors[i].pin1, pwmFreq, pwmRes);
        ledcAttach(motors[i].pin2, pwmFreq, pwmRes);
        pinMode(motors[i].enPin, OUTPUT); // enable pin
      case ONE_PWM_DRIVER:
        // 1 pwm pin for speed and 1 digital pin for direction
        ledcAttach(motors[i].pin1, pwmFreq, pwmRes); // pwm pin
        pinMode(motors[i].pin2, OUTPUT);  // direction pin
        break;
      case ONE_PWM_EN_DRIVER:
        // 1 pwm pin for speed and 1 digital pin for direction and 1 enable pin
        ledcAttach(motors[i].pin1, pwmFreq, pwmRes); // pwm pin
        pinMode(motors[i].pin2, OUTPUT);  // direction pin
        pinMode(motors[i].enPin, OUTPUT); // enable pin
        break;

      default:
        break;
    }
  }
}