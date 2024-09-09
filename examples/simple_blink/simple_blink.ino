#include "RCX32.h"

RCX_Lights lights;

// Pin definitions
#define ledPin1 2  // LED connected to digital pin 2
#define ledPin2 48 // LED connected to digital pin 4

void setup() {
  // add LEDs to the RCX32 object
  // first argument is the type of LED (HEAD_LIGHT, TAIL_LIGHT, etc.) currently
  // only supports predefined types second argument is the pin number the LED is
  // connected to
  lights.addLed(HEAD_LIGHT, ledPin1);
  lights.addLed(TAIL_LIGHT, ledPin2);
}

void loop() {
  // blink the LEDs
  // blink the headlight led for 800ms, then off for 200ms
      lights.blink(HEAD_LIGHT, 800, 200);
  // blink the taillight led for 500ms, then off for 500ms
  lights.blink(TAIL_LIGHT, 500, 500);

  // a small delay to ensure the LEDs don't blink too fast
  delay(1); // added for stability
}
