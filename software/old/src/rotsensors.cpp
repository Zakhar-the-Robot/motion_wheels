#include "rotsensors.h"

RotSensor rs_l(LEFT_ENCODER_PIN, "Left", millis);
RotSensor rs_r(RIGHT_ENCODER_PIN, "Right", millis);

void rotsensors_init(void) {
    attachInterrupt(digitalPinToInterrupt(rs_l.GetPin()), rs_l.GetIntrHandler(), FALLING);
    attachInterrupt(digitalPinToInterrupt(rs_r.GetPin()), rs_r.GetIntrHandler(), FALLING);
    printf("[RS] Ready!\n");
}