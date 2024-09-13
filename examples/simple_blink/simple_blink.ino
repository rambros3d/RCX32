#include "RCX32.h"

RCX_Lights lights;

// Pin definitions
#define ledPin1 -2  // LED connected to digital pin 2
#define ledPin2 48  // LED connected to digital pin 4
#define ledPin3 14  // LED connected to digital pin 14

void setup() {
  // add LEDs to the RCX32 object
  // first argument is the type of LED (HEAD_LIGHT, TAIL_LIGHT, etc.)
  lights.addLed(LED1, ledPin1,100);
  lights.addLed(LED1, ledPin2,100);  // Multiple leds can be added to one type
  lights.addLed(LED2, ledPin3,100);  // Controlling LED2 will contol both ledPin2 & ledPin3
}

void loop() {
  lights.turnOn(LED1);
  delay(500);
  lights.turnOff(LED1);
  delay(500);
}
