#include <Arduino.h>
#ifndef RCX_MOTOR_H
#define RCX_MOTOR_H

#define DefaultPwmFrequency 24000
#define DefaultPwmResolution 10
class Motor2pwm {
  public:
    Motor2pwm(uint8_t pin1, uint8_t pin2, uint16_t pwmFrequency = DefaultPwmFrequency,uint8_t pwmResoltuion = DefaultPwmResolution);
    void init();
    void set(uint8_t speed, bool direction);
    void setResolution(uint8_t pwmResolution);
    void setSpeed(uint8_t speed);
    void setDirection(bool direction);
    bool getDirection() const;
    uint8_t getSpeed() const;
    void stop();
    void coast();

  private:
    uint8_t _pin1;
    uint8_t _pin2;
    uint8_t _speed;
    bool _direction;
    uint16_t _pwmFrequency;
    uint8_t _pwmResolution;
};
#endif