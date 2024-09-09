#include "RCX_motor.h"

Motor2pwm::Motor2pwm(uint8_t pin1, uint8_t pin2, uint16_t pwmFrequency,uint8_t pwmResolution) {
  _pin1 = pin1;
  _pin2 = pin2;
  _speed = 0;
  _direction = false;
  _pwmFrequency = pwmFrequency;
  _pwmResolution = pwmResolution;
}

void Motor2pwm::init() {
  ledcAttach(_pin1, _pwmFrequency, _pwmResolution);
  ledcAttach(_pin2, _pwmFrequency, _pwmResolution);
}

void Motor2pwm::set(uint8_t speed, bool direction) {
  _speed = speed;
  _direction = direction;
  if (_direction) {
    ledcWrite(_pin1, _speed);
    ledcWrite(_pin2, 0);
  } else {
    ledcWrite(_pin1, 0);
    ledcWrite(_pin2, _speed);
  }
}
void Motor2pwm::coast() {  
  ledcWrite(_pin1, (1 << _pwmResolution) - 1);
  ledcWrite(_pin2, (1 << _pwmResolution) - 1);
  _direction=0;
  _speed = 0;
}

void Motor2pwm::setSpeed(uint8_t speed) {
  _speed = speed;
  set(_speed, _direction);
}

void Motor2pwm::setDirection(bool direction) {
  _direction = direction;
  set(_speed, _direction);
}

bool Motor2pwm::getDirection() const {
  return _direction;
}

uint8_t Motor2pwm::getSpeed() const {
  return _speed;
}

void Motor2pwm::stop() {
  _direction=0;
  _speed = 0;
  set(_speed, _direction);
}



