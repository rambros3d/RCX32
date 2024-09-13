// RCX_Lights.cpp
#include "RCX_lights.h"

RCX_Lights::RCX_Lights() {
  leds = nullptr;
  numLeds = 0;
  ledResoltuion = ((1 << DefaultLedResolution) - 1);
}
void RCX_Lights::addLed(LedType type, int8_t pin, int8_t brightness) {
  Led *newLeds = (Led *)realloc(leds, (numLeds + 1) * sizeof(Led));
  if (newLeds == nullptr) {
    return;  // Handle memory allocation error
  }
  leds = newLeds;
  leds[numLeds].type = type;
  if (pin < 0) {
    leds[numLeds].invert = true;
    pin = -pin;
  } else {
    leds[numLeds].invert = false;
  }
  leds[numLeds].ledState = 0;
  leds[numLeds].pin = pin;
  leds[numLeds].ledDutyCycle = (brightness * ledResoltuion / 100);
  numLeds++;
  ledcAttachChannel(pin, DefaultLedFrequency, DefaultLedResolution, numLeds);
  if (leds[numLeds].invert) ledcOutputInvert(pin, true);  // use ledcOutputInvert functionality
}

void RCX_Lights::turnOn(LedType type) {
  updateLed(type, HIGH);
}

void RCX_Lights::turnOff(LedType type) {
  updateLed(type, LOW);
}

void RCX_Lights::fadeOn(LedType type, uint16_t fadeTime) {
  updateLed(type, HIGH, fadeTime);
}

void RCX_Lights::fadeOff(LedType type, uint16_t fadeTime) {
  updateLed(type, LOW, fadeTime);
}

void RCX_Lights::setBrightness(LedType type, int8_t brightness) {
  for (uint8_t i = 0; i < numLeds; i++) {
    if (leds[i].type == type) {
      leds[i].ledDutyCycle = (brightness * ledResoltuion / 100);
    }
  }
}

bool RCX_Lights::getLedState(LedType type) {
  uint8_t ledNum = type;
  return leds[ledNum].ledState;
}

void RCX_Lights::blink(LedType type, uint16_t onTime, uint16_t offTime) {
}


void RCX_Lights::updateLed(LedType type, bool ledstate, uint16_t fadeTime) {
  for (uint8_t i = 0; i < numLeds; i++) {
    if (leds[i].type == type) {
      updateLed(i, ledstate, fadeTime);
    }
  }
}
void RCX_Lights::updateLed(uint8_t led_id, bool ledstate, uint16_t fadeTime) {
  leds[led_id].ledState = ledstate;
  if (fadeTime == 0) {
    ledcWrite(leds[led_id].pin, (ledstate ? 0 : leds[led_id].ledDutyCycle));
  } else {
    ledcFade(leds[led_id].pin, (ledstate ? 0 : leds[led_id].ledDutyCycle), (ledstate ? leds[led_id].ledDutyCycle : 0), fadeTime);
  }
}

void RCX_Lights::breathe(LedType type, int16_t breatheTime, bool defaultState, bool repeatAgain) {
}


void RCX_Lights::breathing(LedType type, int16_t breatheTime, int16_t delayTime) {
}