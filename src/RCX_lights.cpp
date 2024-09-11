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
  // if(leds[numLeds].invert) ledcOutputInvert(pin, true); // there is an error with this ledcOutputInvert
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
  uint8_t ledNum = type;
  return leds[ledNum].ledState;
}

void RCX_Lights::blink(LedType type, uint16_t onTime, uint16_t offTime) {
  // This function blinks a single led on and off at a given rate.
  // It keeps track of the last time the led state was changed,
  // and only changes the state if the time since the last change
  // is greater than the given onTime or offTime.

  // The led to blink is identified by the LedType enum.
  // The LedType is used as an index into the array of led objects.
  uint8_t ledNum = type;
  uint32_t currentTime = millis();

  // If the led is currently on, change it to off if enough time has passed.
  if (leds[ledNum].ledState) {
    if (currentTime - leds[ledNum].timeElapsed >= onTime) {
      // Set the led state to off, and update the time elapsed.
      leds[ledNum].ledState = false;
      leds[ledNum].timeElapsed = currentTime;
      // Call the updateLed function to set the actual led state.
      updateLed(type, false);
    }
  } else {
    // If the led is currently off, change it to on if enough time has passed.
    // If offTime is 0, use the same time as onTime.
    if (offTime == 0) offTime = onTime;
    if (currentTime - leds[ledNum].timeElapsed >= offTime) {
      // Set the led state to on, and update the time elapsed.
      leds[ledNum].ledState = true;
      leds[ledNum].timeElapsed = currentTime;
      // Call the updateLed function to set the actual led state.
      updateLed(type, true);
    }
  }
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
    ledcWrite(leds[led_id].pin,
              invertPwm(ledstate ? leds[led_id].ledDutyCycle : 0,
                        leds[led_id].invert));
  } else {
    ledcFade(leds[led_id].pin,
             invertPwm(ledstate ? 0 : leds[led_id].ledDutyCycle,
                       leds[led_id].invert),
             invertPwm(ledstate ? leds[led_id].ledDutyCycle : 0,
                       leds[led_id].invert),
             fadeTime);
  }
}

void RCX_Lights::breathe(LedType type, int16_t breatheTime, bool defaultState, bool repeatAgain) {
}


void RCX_Lights::breathing(LedType type, int16_t breatheTime, int16_t delayTime) {
}