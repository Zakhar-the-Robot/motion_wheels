#include <Arduino.h>
#include "RotSensor.h"

#define LEFT_ENCODER_PIN 2
#define RIGHT_ENCODER_PIN 3

extern RotSensor rs_l;
extern RotSensor rs_r;

void rotsensors_init(void);