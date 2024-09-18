#include "RCX32.h"

// Pin definitions
#define ledPin1 2   // LED connected to digital pin 2
#define ledPin2 48  // LED connected to digital pin 4
#define ledPin3 14  // LED connected to digital pin 14
#define ledPin4 15  // LED connected to digital pin 14

// Initialize two RCX_lights objects
// One for the LED on pin 2, One for the LEDs on pins 4 and 14
// upto 4 leds can be initialized as one light
RCX_lights LIGHT1(ledPin1);
RCX_lights LIGHT2(ledPin2, ledPin3);
// RCX_lights MULTIPLE_LEDS(ledPin1, ledPin2, ledPin3, ledPin4);

void setup() {
  // Initialize the serial monitor
  Serial.begin(115200);

  // Initialize the two LED lights
  LIGHT1.init();
  LIGHT2.init();

  // Set the second light to blink at 300ms
  LIGHT2.blink(300);

  // Initialize the CoroutineScheduler
  CoroutineScheduler::setup();

  // Set the first light to blink at 400ms
  LIGHT1.attachPWM();
  //LIGHT1.breathing(1000);
  LIGHT1.candle(20);
}

void loop() {

  CoroutineScheduler::loop();
  delay(1);  // added for esp32 stability
}

/*

  LIGHT1._applyFade(HIGH, 500);
  delay(1000);
  LIGHT1._applyFade(LOW, 500);
  delay(1000);

  ledcFade(2, 0, 1023, 500);
  delay(1000);
  ledcFade(2, 1023, 0, 500);
  delay(1000);


  */