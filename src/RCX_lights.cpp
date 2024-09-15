// RCX_Lights.cpp
#include "RCX_lights.h"

RCX_Lights::RCX_Lights() {
  //leds = nullptr;
  numLeds = 0;
  ledResoltuion = ((1 << DefaultLedResolution) - 1);
}

void RCX_Lights::addLed(LedType type, int8_t pin) {
  // search for the first free pin in the type array
  for (uint8_t i = 0; i < 4; i++) {
    if (leds[type].pin[i] == 127) {
      // save the pin and break the loop
      leds[type].pin[i] = pin;
      break;
    }
  }
  // set the channel to invalid
  leds[type].channel = 127;
  // set the pin mode to output
  pinMode(abs(pin), OUTPUT);
}

void RCX_Lights::removeLed(int8_t pin) {
  for (uint8_t i = 0; i < 10; i++) {
    for (uint8_t j = 0; j < 4; j++) {
      if (leds[i].pin[j] == pin) {
        // remove the pin and break the loop
        ledcDetach(abs(leds[i].pin[j]));
        leds[i].pin[j] = 127;
        break;
      }
    }
  }
}

void RCX_Lights::_attachPWM(LedType type) {
  if (leds[type].channel != 127) return;
  uint8_t channel = 0;
  while (channelBitMask & (1 << channel)) {
    channel++;
  }

  for (uint8_t i = 0; i < 4; i++) {
    if (leds[type].pin[i] != 127) {
      leds[type].channel = channel;
      ledcAttachChannel(abs(leds[type].pin[i]), DefaultLedFrequency, DefaultLedResolution, channel);
      if (leds[type].pin[i] < 0) ledcOutputInvert(abs(leds[type].pin[i]), true);
    }
  }
  channelBitMask |= (1 << channel);
}
void RCX_Lights::_detachPWM(LedType type) {
}

void RCX_Lights::digitalState(LedType type, bool state) {
  if (leds[type].channel != 127) _detachPWM(type);
  for (auto &pin : leds[type].pin) {
    if (pin != 127) {
      if (pin < 0) {
        digitalWrite(abs(pin), !state);
      } else {
        digitalWrite(pin, state);
      }
    }
  }
}
void RCX_Lights::turnOn(LedType type) {
  digitalState(type, true);
}
void RCX_Lights::turnOff(LedType type) {
  digitalState(type, false);
}

void RCX_Lights::setBrightness(LedType type, uint8_t brightness) {
  uint16_t _ledDutyCycle = (brightness * ledResoltuion) / 100;
  setPwmDuty(type, _ledDutyCycle);
}
void RCX_Lights::setPwmDuty(LedType type, uint16_t ledDutyCycle) {
  uint8_t channel = leds[type].channel;
  if (channel == 127) _attachPWM(type);
  ledcWriteChannel(channel, ledDutyCycle);
}

void RCX_Lights::applyFade(LedType type, bool state, uint16_t fadeTime) {
  if (leds[type].channel == 127) _attachPWM(type);
  if (state == 1) {
    ledcFade(leds[type].pin[0], 0, ledResoltuion, fadeTime);
  } else {
    ledcFade(leds[type].pin[0], ledResoltuion, 0, fadeTime);
  }
}

void RCX_Lights::fadeOn(LedType type, uint16_t fadeTime) {
  applyFade(type, HIGH, fadeTime);
}

void RCX_Lights::fadeOff(LedType type, uint16_t fadeTime) {
  applyFade(type, LOW, fadeTime);
}
void RCX_Lights::breathe(LedType type, int16_t breatheTime, bool defaultState, bool repeatAgain) {
}
void RCX_Lights::breathing(LedType type, int16_t breatheTime, int16_t delayTime) {
}
void RCX_Lights::blink(LedType type, uint16_t onTime, uint16_t offTime) {
}