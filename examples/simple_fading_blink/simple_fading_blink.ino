#include "RCX32.h"

RCX_Lights lights;

// Pin definitions
#define ledPin1 2  // LED connected to digital pin 2
#define ledPin2 4  // LED connected to digital pin 4
#define ledPin3 14 // LED connected to digital pin 14

void setup() {
  // add LEDs to the RCX32 object
  // first argument is the type of LED (HEAD_LIGHT, TAIL_LIGHT, etc.) currently
  // only supports predefined types second argument is the pin number the LED is
  // connected to
  lights.addLed(LED1, ledPin1);
  lights.addLed(LED1, ledPin2); // Multiple leds can be added to one type
  lights.addLed(LED2,
                ledPin3); // Controlling LED2 will contol both ledPin2 & ledPin3
}

void loop() {
  // blink the LEDs
  // blink the headlight led for 800ms, then off for 200ms
  lights.breathing(LED1, 1000);

  // a small delay to ensure the LEDs don't blink too fast
  delay(1); // added for stability
}
