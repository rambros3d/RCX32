// RCX_Lights.h
#include <Arduino.h>
#ifndef RCX_LIGHTS_H
#define RCX_LIGHTS_H

#define DefaultLedFrequency 4000
#define DefaultLedResolution 10
#define DefaultFadeTime 500

#define maxChannels 8   // ESP32S3 has only 8 ledc channels
#define maxLedTypes 15  // ESP32S3 has only 8 ledc channels

enum LedType : uint8_t {
  // common lights
  HEAD_LIGHT_ID = 1,
  FULLBEAM_LIGHT_ID = 2,
  TAIL_LIGHT_ID = 3,
  BRAKE_LIGHT_ID = 4,
  LEFT_INDICATOR_ID = 5,  // left and right indicator can use the same ledc channel
  RIGHT_INDICATOR_ID = 6,
  CAB_LIGHT_ID = 7,
  FOG_LIGHT_ID = 8,
  REVERSE_LIGHT_ID = 9,
  // used in trucks
  SIDE_LIGHT_ID = 10,
  BEACON_LIGHT_ID = 11,
  HAZARD_LIGHT_ID = 12,
  // exclusively used in locomotives
  WALKWAY_LIGHT_ID = 13,
  DITCH_LIGHT_ID = 14,
  // extra leds with same types
  LED1 = 1,
  LED2 = 2,
  LED3 = 3,
  LED4 = 4,
  LED5 = 5,
  LED6 = 6,
  LED7 = 7,
  LED8 = 8,
  LED9 = 9,
  LED10 = 10,
};

enum LedState : uint8_t {
  ON = 1,
  OFF = 0,
};
class RCX_Lights {
public:
  RCX_Lights();
  void addLed(LedType type, int8_t pin);

  void turnOn(LedType type);
  void turnOff(LedType type);
  void digitalState(LedType type, bool state);

  void fadeOn(LedType type, uint16_t fadeTime = DefaultFadeTime);
  void fadeOff(LedType type, uint16_t fadeTime = DefaultFadeTime);
  void applyFade(LedType type, bool state, uint16_t fadeTime);

  void setBrightness(LedType type, uint8_t brightness);
  void setPwmDuty(LedType type, uint16_t ledDutyCycle);
  void setFrequency(LedType type, int16_t ledFrequency = DefaultLedFrequency);

  void blink(LedType type, uint16_t onTime = 1000, uint16_t offTime = 0);
  void breathe(LedType type, int16_t breatheTime = 1000, bool defaultState = 1, bool repeatAgain = 0);
  void breathing(LedType type, int16_t breatheTime = 1000, int16_t delayTime = 0);

  int8_t getChannel(LedType type);
  void printDebug();  // Debug

  struct Led {
    int8_t pin[4] = { 127, 127, 127, 127 };  // 4 leds can be attached to same type
    int8_t channel = 127;
  };
  Led leds[maxLedTypes];  //  upto 15 types can be used at once

  struct LedChannels {
    LedType type;
    int16_t onTime;
    int16_t offTime;
    int16_t smoothTime = 500;
    int16_t ledDutyCycle;
    int16_t ledFrequency;
  };
  LedChannels chData[maxChannels];

private:
  void _attachPWM(LedType type);
  void _detachPWM(LedType type);
  int numLeds;
  int16_t ledResoltuion;
  uint16_t channelBitMask = 0b0000000000000000;  // track used ledc channels
};

#endif
