// RCX_Lights.h
#include <Arduino.h>
#ifndef RCX_LIGHTS_H
#define RCX_LIGHTS_H

#include <AceRoutine.h>
using namespace ace_routine;

#define DefaultLedFrequency 4000
#define DefaultLedResolution 10
#define DefaultFadeTime 500

#define maxChannels 8  // ESP32S3 has only 8 ledc channels
#define maxLeds 4      // max number os led pins per constructor

class RCX_lights : public Coroutine {
public:
  RCX_lights(int8_t pin1, int8_t pin2 = 127, int8_t pin3 = 127, int8_t pin4 = 127);
  void init();
  void turnOn();
  void turnOff();
  void setState(bool state);

  void fadeOn(uint16_t fadeTime = DefaultFadeTime);
  void fadeOff(uint16_t fadeTime = DefaultFadeTime);
  void attachPWM();
  void detachPWM();

  void _applyFade(bool state, uint16_t fadeTime);

  void blink(uint16_t onTime = 500);
  void blink(uint16_t onTime, uint16_t offTime);
  void fadeOnce(int16_t breatheTime = 1000);
  void breathing(int16_t breatheTime = 1000, int16_t delayTime = 0);
  /* 
  void setBrightness(uint8_t brightness);
  void setPwmDuty(uint16_t ledDutyCycle);
  void setFrequency(int16_t ledFrequency = DefaultLedFrequency);
  int8_t getChannel();
  void printDebug();  // Debug */
  int runCoroutine() override;

private:
  void _switchLed(bool state);
  void _fadeOnLed(int16_t fadeTime);
  void _fadeOffLed(int16_t fadeTime);
  int8_t _pins[4];
  int8_t _channel;
  int8_t _ledState;
  int16_t _onTime;
  int16_t _offTime;
  int16_t _smoothOnTime;
  int16_t _smoothOffTime;
  int16_t _repeat;
  int16_t _ledDutyCycle;
  int16_t _ledFrequency;
  int16_t _ledResoltuion;
};

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
};
enum LedStates : uint8_t {
  OFF = 0,
  ON = 1,
  FADE = 2,
  BLINKING = 3,
  BREATHING = 4,
};

#endif