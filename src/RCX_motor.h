// RCX_motor.h
#include <Arduino.h>
#ifndef RCX_MOTOR_H
#define RCX_MOTOR_H

#define DefaultPwmFrequency 24000
#define DefaultPwmResolution 10
#define DefaultControlResolution 100

enum MotorType : uint8_t {
  DRIVER_MOTOR,
  STEERING_MOTOR,
  MOTOR1,
  MOTOR2,
  MOTOR3,
  MOTOR4,
  MOTOR5,
  MOTOR6
};
enum MotorDir : bool {
  FORWARD = 1,
  REVERSE = 0,
  BACKWARD = 0,
};
enum DriverType : uint8_t {
  // driver types
  TWO_PWM_DRIVER,
  TWO_PWM_EN_DRIVER,
  ONE_PWM_DRIVER,
  ONE_PWM_EN_DRIVER,
};

struct Motor {
  MotorType type;
  DriverType driverType;
  uint8_t motorId;
  int8_t pin1;
  int8_t pin2;
  int8_t pin3;
  bool invertPin1;
  bool invertPin2;
  bool invertPin3;
  int16_t motorSpeed;
  bool motorDirection;
  bool motorState;
  uint32_t timeElapsed = 0;
};

class RCX_Motors {
public:
  RCX_Motors();
  void add2pwmDriver(MotorType type, int8_t pwmPin1, int8_t pwmPin2, int8_t enablePin = 0);
  void add1pwmDriver(MotorType type, int8_t pwmPin, int8_t dirPin, int8_t enablePin = 0);
  void setSpeed(MotorType type, int16_t inputSpeed, bool inputDirection = 1);
  void init(int16_t pwmFrequency = DefaultPwmFrequency, int16_t pwmResolution = DefaultPwmResolution);
  void run(MotorType type, int16_t inputSpeed, bool inputDirection = 1);
  void runMotorId(uint8_t motorNum, int16_t inputSpeed, bool inputDirection = 1);
  void update(MotorType type);
  void updateAll();

private:
  Motor *motors;
  int8_t numMotors;
  int16_t pwmResoltuion;
};

#endif
