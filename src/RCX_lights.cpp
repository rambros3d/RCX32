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
    if (leds[type].pin[i] == pin) break;
    else if (leds[type].pin[i] == 127) {
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
  if (leds[type].channel == 127) return;
  channelBitMask &= ~(1 << leds[type].channel);

  for (auto &pin : leds[type].pin) {
    if (pin != 127) {
      ledcDetach(abs(pin));
    }
  }
}

void RCX_Lights::digitalState(LedType type, bool state) {
  // If this LED has a PWM channel associated with it, detach it.
  if (leds[type].channel != 127) _detachPWM(type);

  // Iterate over all the pins associated with this LED type.
  for (auto &pin : leds[type].pin) {
    // If the pin is not set to 127 (which means that the pin
    //is not used), then set the pin to the desired state.
    if (pin != 127) {
      // If the pin is negative, then we need to invert the state
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
  // If the channel has not been initialized yet, attach it.
  if (leds[type].channel == 127) _attachPWM(type);
  // Fade the LED to the desired state.
  if (state == 1) {
    ledcFade(abs(leds[type].pin[0]), 0, ledResoltuion, fadeTime);
  } else {
    ledcFade(abs(leds[type].pin[0]), ledResoltuion, 0, fadeTime);
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

int8_t RCX_Lights::getChannel(LedType type) {
  return leds[type].channel;
}

void RCX_Lights::printDebug() {
  Serial.println("LEDs:");
  for (uint8_t i = 0; i < maxLedTypes; i++) {
    Serial.print("Type:");
    Serial.print(i);
    Serial.print("\t  Channel: ");
    if (leds[i].channel == 127) {
      Serial.println("Not used");
    } else {
      Serial.print(leds[i].channel);
      Serial.print("\tPins: ");
      for (auto &pin : leds[i].pin) {
        if (pin != 127) {
          Serial.print(pin);
          Serial.print(", ");
        }
      }
      Serial.println();
    }
  }
}
