// RCX_Lights.h
#include <Arduino.h>
#ifndef RCX_LIGHTS_H
#define RCX_LIGHTS_H

#define DefaultLedFrequency 4000
#define DefaultLedResolution 8
#define DefaultFadeTime 500

// ESP32S3 has only 8 ledc channels
enum LedType {
  // common lights
  HEAD_LIGHT,
  FULLBEAM_LIGHT,
  TAIL_LIGHT,
  BRAKE_LIGHT,
  LEFT_INDICATOR,  // left and right indicator will use the same ledc channel
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
enum LedStates : uint8_t {
  LED_OFF = 0,
  LED_ON = 1,
  LED_BLINKING = 2,
  LED_BREATHING = 3,
};

struct Led {
  LedType type;
  int8_t pin;
  bool invert;
  bool ledState;
  uint8_t brightness;
  int16_t onTime;
  int16_t offTime;
  int16_t smoothTime = 500;
  int16_t ledDutyCycle;
  int16_t ledFrequency;
};

class RCX_Lights {
public:
  RCX_Lights();
  void addLed(LedType type, int8_t pin);
  void addLed(LedType type, int8_t pin, int8_t brightness);

  void updateLed(LedType type, bool ledstate, uint16_t fadeTime = 0);
  void updateLed(uint8_t led_id, bool ledstate, uint16_t fadeTime = 0);

  void turnOn(LedType type);
  void turnOff(LedType type);

  void fadeOn(LedType type, uint16_t fadeTime = DefaultFadeTime);
  void fadeOff(LedType type, uint16_t fadeTime = DefaultFadeTime);

  void setBrightness(LedType type, int8_t brightness);
  void setFrequency(LedType type, int16_t ledFrequency = DefaultLedFrequency);

  void blink(LedType type, uint16_t onTime = 1000, uint16_t offTime = 0);
  void breathe(LedType type, int16_t breatheTime = 1000, bool defaultState = 1, bool repeatAgain = 0);
  void breathing(LedType type, int16_t breatheTime = 1000, int16_t delayTime = 0);

  bool getLedState(LedType type);

private:
  Led *leds;
  int8_t numLeds;
  int16_t ledResoltuion;
};

#endif
