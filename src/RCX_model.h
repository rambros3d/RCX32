#include <Arduino.h>
#ifndef RCX_MODELS_H
#define RCX_MODELS_H

enum ModelType : uint8_t {
  RC_TRUCK,
  RC_CAR,
  RC_EXCAVATOR,
  RC_LOCOMOTIVE,
  RC_TANK,
  RC_TRAIN,
  RC_SCOOTER,
};

class RCX_Model {
public:
  RCX_Model();
};

#endif