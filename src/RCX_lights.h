// RCX_Lights.h
#include <Arduino.h>
#ifndef RCX_LIGHTS_H
#define RCX_LIGHTS_H

#define DefaultLedFrequency 4000
#define DefaultLedResolution 8
#define DefaultFadeTime 500

enum LedType {
  // common lights
  HEAD_LIGHT,
  FULLBEAM_LIGHT,
  TAIL_LIGHT,
  BRAKE_LIGHT,
  LEFT_INDICATOR,
  RIGHT_INDICATOR,
  CAB_LIGHT,
  FOG_LIGHT,
  REVERSE_LIGHT,
  // used in trucks
  SIDE_LIGHT,
  BEACON_LIGHT,
  HAZARD_LIGHT,
  // exclusively used in locomotives
  WALKWAY_LIGHT,
  DITCH_LIGHT,
  // extra leds
  LED1,
  LED2,
  LED3,
  LED4,
  LED5,
  LED6,
};

struct Led {
  LedType type;
  int8_t pin;
  bool invert;
  int16_t ledDutyCycle;
  int16_t ledFrequency;
  bool ledState;
  uint32_t timeElapsed = 0;
};

class RCX_Lights {
public:
  RCX_Lights();
  void addLed(LedType type, int8_t pin, int8_t brightness = 100);
  void updateLed(LedType type, bool ledstate, uint16_t fadeTime = 0);
  void updateLed(uint8_t LedNum, bool ledstate, uint16_t fadeTime = 0);
  void turnOn(LedType type);
  void turnOff(LedType type);
  void fadeOn(LedType type, uint16_t fadeTime = DefaultFadeTime);
  void fadeOff(LedType type, uint16_t fadeTime = DefaultFadeTime);
  void setBrightness(LedType type, int8_t brightness);
  void setFrequency(LedType type, int16_t ledFrequency = DefaultLedFrequency);

  void blink(LedType type, uint16_t onTime = 1000, uint16_t offTime = 0);
  void quickBlink(uint8_t ledNum, uint16_t onTime = 1000, uint16_t offTime = 0);

  bool getLedState(LedType type);
  bool getLedState(uint8_t ledNum);

private:
  uint16_t invertPwm(uint16_t dutyCycle, bool invert = 0);
  Led *leds;
  int8_t numLeds;
  int16_t ledResoltuion;
};

#endif
