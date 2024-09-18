// RCX_Lights.cpp
#include "RCX_lights.h"

uint16_t channelBitMask = 0b0000000000000000;  // track used ledc channels

RCX_lights::RCX_lights(int8_t pin1, int8_t pin2, int8_t pin3, int8_t pin4) {
  _pins[0] = pin1;
  _pins[1] = pin2;
  _pins[2] = pin3;
  _pins[3] = pin4;
  _ledResoltuion = DefaultLedResolution;
  _ledDutyCycle = ((1 << DefaultLedResolution) - 1);
  _ledFrequency = DefaultLedFrequency;
}

void RCX_lights::init() {
  for (uint8_t i = 0; i < maxLeds; i++) {
    if (_pins[i] == 127) return;
    pinMode(abs(_pins[i]), OUTPUT);
  }
  _channel = 0;
  _ledState = 0;
  _onTime = 0;
  _offTime = 0;
  _smoothOnTime = 0;
  _smoothOffTime = 0;
  _repeat = 0;
}

void RCX_lights::turnOn() {
  _ledState = ON;
}
void RCX_lights::turnOff() {
  _ledState = OFF;
}

void RCX_lights::fadeOn(uint16_t fadeTime) {
  _smoothOnTime = fadeTime;
  _smoothOffTime = 0;
  _ledState = FADE;
}

void RCX_lights::fadeOff(uint16_t fadeTime) {
  _smoothOffTime = fadeTime;
  _smoothOnTime = 0;
  _ledState = FADE;
}


void RCX_lights::blink(uint16_t onTime) {
  _onTime = onTime;
  _offTime = onTime;
  _ledState = BLINKING;
}

void RCX_lights::blink(uint16_t onTime, uint16_t offTime) {
  _onTime = onTime;
  _offTime = offTime;
  _ledState = BLINKING;
}
void RCX_lights::fadeOnce(int16_t breatheTime) {
}
void RCX_lights::breathing(int16_t breatheTime, int16_t delayTime) {
  _onTime = delayTime;
  _offTime = delayTime;
  _smoothOnTime = breatheTime;
  _smoothOffTime = breatheTime;
  _ledState = BREATHING;
}


void RCX_lights::attachPWM() {

  for (uint8_t i = 0; i < maxLeds; i++) {
    if (_pins[i] == 127) break;
    _channel = 1;
    bool success = ledcAttachChannel(abs(_pins[i]), _ledFrequency, _ledResoltuion, _channel);
    if (_pins[i] < 0) ledcOutputInvert(abs(_pins[i]), true);
  }
}
void RCX_lights::detachPWM() {
  for (uint8_t i = 0; i < maxLeds; i++) {
    if (_pins[i] == 127) break;
    ledcDetach(abs(_pins[i]));
  }
}

int RCX_lights::runCoroutine() {
  COROUTINE_LOOP() {
    switch (_ledState) {
      case OFF:
        _switchLed(LOW);
        break;
      case ON:
        _switchLed(HIGH);
        break;
      case FADE:
        attachPWM();
        if (_smoothOnTime > 0) _applyFade(ON, _smoothOnTime);
        else _applyFade(OFF, _smoothOffTime);
        COROUTINE_DELAY(_smoothOffTime);
        detachPWM();
        break;
      case BLINKING:
        _switchLed(HIGH);
        COROUTINE_DELAY(_onTime);
        _switchLed(LOW);
        COROUTINE_DELAY(_offTime);
        break;
      case BREATHING:
        _applyFade(ON, _smoothOnTime);
        COROUTINE_DELAY(_smoothOnTime);
        _applyFade(OFF, _smoothOffTime);
        COROUTINE_DELAY(_smoothOffTime);
        break;
      default:
        break;
    }
  }
}

// private functions

void RCX_lights::_switchLed(bool state) {
  for (uint8_t i = 0; i < maxLeds; i++) {
    if (_pins[i] == 127) break;
    if (_pins[i] >= 0) digitalWrite(_pins[i], state);
    else digitalWrite(abs(_pins[i]), !state);
  }
}
void RCX_lights::_applyFade(bool state, uint16_t fadeTime) {
  if (state == ON) ledcFade(abs(_pins[0]), 0, _ledDutyCycle, fadeTime);
  else ledcFade(abs(_pins[0]), _ledDutyCycle, 0, fadeTime);
}