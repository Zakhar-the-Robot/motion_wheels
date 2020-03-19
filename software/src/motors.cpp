#include <Arduino.h>
#include "motors.h"
#include "common_config.h"

// =============================================================================
// implementations
// ========================================================================= vvv

void set_pin(int pin, bool value)
{
    digitalWrite((uint8_t)pin, (uint8_t)value);
}

void init_write_pin(int pin)
{
    pinMode(pin, OUTPUT);
}
// ========================================================================= ^^^


Motors_dc2platform wheels(PINR1, PINR2, PINL1, PINL2);

void motors_greeting(void) {
    printf("[Motors] Ready!\n");

    wheels.MoveBackward();
    delay(300);

    wheels.MoveForward();
    delay(300);
    wheels.Stop();

}

void W(void) {
    wheels.MoveForward();
    delay(MOTORS_STEP_TIMEOUT_MS);
    wheels.Stop();
}
void S(void) {
    wheels.MoveBackward();
    delay(MOTORS_STEP_TIMEOUT_MS);
    wheels.Stop();
}
void A(void) {
    wheels.MoveLeft();
    delay(MOTORS_STEP_TIMEOUT_MS);
    wheels.Stop();
}
void D(void) {
    wheels.MoveRight();
    delay(MOTORS_STEP_TIMEOUT_MS);
    wheels.Stop();
}
void Stop(void) {
    wheels.Stop();
}
