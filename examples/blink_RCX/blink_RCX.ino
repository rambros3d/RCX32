#include "RCX32.h"

RCX_Lights lights;
void setup() {
  Serial.begin(115200);
  delay(100);
  lights.addLed(HEAD_LIGHT, 2,100);
  lights.addLed(TAIL_LIGHT, 48,100);
}

void loop() {

  Serial.println(lights.getLedState(HEAD_LIGHT));
  delay(10);

  lights.blink(HEAD_LIGHT, 200);
  lights.blink(TAIL_LIGHT, 400);
  //lights.blink(TAIL_LIGHT, 600);

}

/*

  lights.turnOn(HEAD_LIGHT);
  delay(1000);
  lights.turnOff(HEAD_LIGHT);
  delay(200);

    lights.fadeOn(HEAD_LIGHT,500);
  delay(2000);
  lights.fadeOff(HEAD_LIGHT,500);
  delay(500);

  lights.blink(HEAD_LIGHT, 1000);
  delay(100);

  void RCX_Lights::blink(LedType type, uint16_t onTime, uint16_t offTime) {
  uint8_t i= getLedNum(type);
  bool ledState=leds[i].ledState;
  uint32_t now = millis();
  if (offTime==0) {    offTime=onTime;  }
  if (now - leds[i].timeElapsed >= 500){ //(ledState ? offTime : onTime)) {
    ledState = !ledState;
    updateLed(i,ledState);
    leds[i].timeElapsed= now;   
  }   
  }

*/