// RCX_motor.cpp
#include "RCX_motor.h"

RCX_Motors::RCX_Motors() {
  motors = nullptr;
  numMotors = 0;
  pwmResoltuion = ((1 << DefaultPwmResolution) - 1);
}

void RCX_Motors::add2pwmDriver(MotorType type, int8_t pwmPin1, int8_t pwmPin2, int8_t enablePin) {
  Motor *newMotors = (Motor *)realloc(motors, (numMotors + 1) * sizeof(Motor));
  if (newMotors == nullptr) {
    return;  // Handle memory allocation error
  }
  motors = newMotors;
  motors[numMotors].motorId = numMotors;
  motors[numMotors].type = type;

  motors[numMotors].invertPin1 = (pwmPin1 < 0) ? true : false;
  motors[numMotors].invertPin2 = (pwmPin2 < 0) ? true : false;
  motors[numMotors].pin1 = abs(pwmPin1);
  motors[numMotors].pin2 = abs(pwmPin2);

  if (enablePin == 0) {
    motors[numMotors].driverType = TWO_PWM_DRIVER;
  } else {
    motors[numMotors].invertPin3 = (enablePin < 0) ? true : false;
    motors[numMotors].pin3 = abs(enablePin);
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
  if (dirPin != 0) motors[numMotors].pin2 = abs(dirPin);

  if (enablePin == 0) {
    motors[numMotors].driverType = ONE_PWM_DRIVER;
  } else {
    motors[numMotors].invertPin3 = (enablePin < 0) ? true : false;
    motors[numMotors].pin3 = abs(enablePin);
    motors[numMotors].driverType = ONE_PWM_EN_DRIVER;
  }
  numMotors++;
}

void RCX_Motors::setSpeed(MotorType type, int16_t inputSpeed, bool inputDirection) {
  for (uint8_t i = 0; i < numMotors; i++) {
    if (motors[i].type == type) {
      motors[i].motorEnabled = true;
      motors[i].motorSpeed = inputSpeed;
      motors[i].motorDirection = inputDirection;
    }
  }
}

void RCX_Motors::updateAll() {
  for (uint8_t i = 0; i < numMotors; i++) {
    runMotorId(i, motors[i].motorSpeed, motors[i].motorDirection);
  }
}

void RCX_Motors::update(MotorType type) {
  for (uint8_t i = 0; i < numMotors; i++) {
    if (motors[i].type == type) {
      run(motors[i].type, motors[i].motorSpeed, motors[i].motorDirection);
    }
  }
}

void RCX_Motors::run(MotorType type, int16_t inputSpeed, bool inputDirection) {
  for (uint8_t i = 0; i < numMotors; i++) {
    if (motors[i].type == type) {
      motors[i].motorEnabled = true;
      runMotorId(i, inputSpeed, inputDirection);
    }
  }
}
void RCX_Motors::stopAll() {
  for (uint8_t i = 0; i < numMotors; i++) {
    motors[i].motorEnabled = false;
    runMotorId(i, 0, true);
  }
}
void RCX_Motors::stop(MotorType type) {
  for (uint8_t i = 0; i < numMotors; i++) {
    if (motors[i].type == type) {
      motors[i].motorEnabled = false;
      runMotorId(i, 0, true);
    }
  }
}

void RCX_Motors::runMotorId(uint8_t motorNum, int16_t inputSpeed, bool inputDirection) {
  if (inputSpeed < 0) {
    inputSpeed = -inputSpeed;
    inputDirection = !inputDirection;
  } else {
    inputDirection = !inputDirection;
  }
  int16_t pwmDuty = inputSpeed;
  pwmDuty = constrain(pwmDuty, 0, pwmResoltuion);

  if (motors[motorNum].motorEnabled) {
    switch (motors[motorNum].driverType) {
      case TWO_PWM_DRIVER:
        if (inputDirection) {
          ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion - pwmDuty : pwmDuty);
          ledcWrite(motors[motorNum].pin2, motors[motorNum].invertPin2 ? pwmResoltuion : 0);
        } else {
          ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion : 0);
          ledcWrite(motors[motorNum].pin2, motors[motorNum].invertPin2 ? pwmResoltuion - pwmDuty : pwmDuty);
        }
        break;
      case TWO_PWM_EN_DRIVER:
        digitalWrite(motors[motorNum].pin3, !motors[motorNum].invertPin3);
        if (inputDirection) {
          ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion - pwmDuty : pwmDuty);
          ledcWrite(motors[motorNum].pin2, motors[motorNum].invertPin2 ? pwmResoltuion : 0);
        } else {
          ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion : 0);
          ledcWrite(motors[motorNum].pin2, motors[motorNum].invertPin2 ? pwmResoltuion - pwmDuty : pwmDuty);
        }
        break;
      case ONE_PWM_DRIVER:
        ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion - pwmDuty : pwmDuty);
        digitalWrite(motors[motorNum].pin2, motors[motorNum].invertPin2 ? !inputDirection : inputDirection);
        break;
      case ONE_PWM_EN_DRIVER:
        digitalWrite(motors[motorNum].pin3, !motors[motorNum].invertPin3);
        ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion - pwmDuty : pwmDuty);
        digitalWrite(motors[motorNum].pin2, motors[motorNum].invertPin2 ? !inputDirection : inputDirection);
        break;
      default:
        break;
    }
  } else {
    switch (motors[motorNum].driverType) {
      case TWO_PWM_DRIVER:
        ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion : 0);
        ledcWrite(motors[motorNum].pin2, motors[motorNum].invertPin2 ? pwmResoltuion : 0);
        break;
      case TWO_PWM_EN_DRIVER:
        ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion : 0);
        ledcWrite(motors[motorNum].pin2, motors[motorNum].invertPin2 ? pwmResoltuion : 0);
        digitalWrite(motors[motorNum].pin3, motors[motorNum].invertPin3);
        break;
      case ONE_PWM_DRIVER:
        ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion : 0);
        digitalWrite(motors[motorNum].pin2, motors[motorNum].invertPin2);
        break;
      case ONE_PWM_EN_DRIVER:
        ledcWrite(motors[motorNum].pin1, motors[motorNum].invertPin1 ? pwmResoltuion : 0);
        digitalWrite(motors[motorNum].pin2, motors[motorNum].invertPin2);
        digitalWrite(motors[motorNum].pin3, motors[motorNum].invertPin3);
        break;
      default:
        break;
    }
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
        pinMode(motors[i].pin3, OUTPUT);  // enable pin
        break;
      case ONE_PWM_DRIVER:
        // 1 pwm pin for speed and 1 digital pin for direction
        ledcAttach(motors[i].pin1, pwmFreq, pwmRes);  // pwm pin
        pinMode(motors[i].pin2, OUTPUT);              // direction pin
        break;
      case ONE_PWM_EN_DRIVER:
        // 1 pwm pin for speed and 1 digital pin for direction and 1 enable pin
        ledcAttach(motors[i].pin1, pwmFreq, pwmRes);  // pwm pin
        pinMode(motors[i].pin2, OUTPUT);              // direction pin
        pinMode(motors[i].pin3, OUTPUT);              // enable pin
        break;

      default:
        break;
    }
  }
}
