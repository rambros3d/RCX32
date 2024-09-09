// RCX_Lights.cpp
#include "RCX_lights.h"

RCX_Lights::RCX_Lights() {
  leds = nullptr;
  numLeds = 0;
  ledResoltuion = (1 << DefaultLedResolution - 1);
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
  ledcAttach(pin, DefaultLedFrequency, DefaultLedResolution);
  // if(leds[numLeds].invert) ledcOutputInvert(pin, true); // there is an error
  // with this ledcOutputInvert
}

uint8_t RCX_Lights::getLedNum(LedType type) {
  for (uint8_t i = 0; i < numLeds; i++) {
    if (leds[i].type == type) {
      return i;
    }
  }
  return 0;
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

void RCX_Lights::updateLed(LedType type, bool ledstate, uint16_t fadeTime) {
  for (uint8_t i = 0; i < numLeds; i++) {
    if (leds[i].type == type) {
      updateLed(i, ledstate, fadeTime);
    }
  }
}
void RCX_Lights::updateLed(uint8_t LedNum, bool ledstate, uint16_t fadeTime) {
  if (fadeTime == 0) {
    ledcWrite(leds[LedNum].pin,
              invertPwm(ledstate ? leds[LedNum].ledDutyCycle : 0,
                        leds[LedNum].invert));
  } else {
    ledcFade(leds[LedNum].pin,
             invertPwm(ledstate ? 0 : leds[LedNum].ledDutyCycle,
                       leds[LedNum].invert),
             invertPwm(ledstate ? leds[LedNum].ledDutyCycle : 0,
                       leds[LedNum].invert),
             fadeTime);
  }
  leds[LedNum].ledState = ledstate;
}

uint16_t RCX_Lights::invertPwm(uint16_t dutyCycle, bool invert) {
  if (invert) {
    return (ledResoltuion - dutyCycle);
  } else {
    return dutyCycle;
  }
}

void RCX_Lights::setBrightness(LedType type, int8_t brightness) {
  for (uint8_t i = 0; i < numLeds; i++) {
    if (leds[i].type == type) {
      leds[i].ledDutyCycle = (brightness * ledResoltuion / 100);
    }
  }
}

bool RCX_Lights::getLedState(LedType type) {
  uint8_t ledNum = getLedNum(type);
  return leds[ledNum].ledState;
}
bool RCX_Lights::getLedState(uint8_t ledNum) {
  return leds[ledNum].ledState;
}

void RCX_Lights::blink(LedType type, uint16_t onTime, uint16_t offTime) {
  quickBlink(getLedNum(type), onTime, offTime);
}

void RCX_Lights::quickBlink(uint8_t ledNum, uint16_t onTime, uint16_t offTime) {
  uint32_t previousMillis = leds[ledNum].timeElapsed;
  if ((millis() - previousMillis) >= onTime) {
    currentState[ledNum] = !currentState[ledNum];
    leds[numLeds].ledState = !leds[numLeds].ledState;
    leds[ledNum].timeElapsed = millis();
    updateLed(ledNum, !currentState[ledNum]);
    // updateLed(ledNum, leds[numLeds].ledState);
  }
}