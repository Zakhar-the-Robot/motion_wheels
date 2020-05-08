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
#if MOTORS_STEP_MODE
    delay(MOTORS_STEP_TIMEOUT_MS);
    wheels.Stop();
#endif
}
void S(void) {
    wheels.MoveBackward();
#if MOTORS_STEP_MODE
    delay(MOTORS_STEP_TIMEOUT_MS);
    wheels.Stop();
#endif
}
void A(void) {
    wheels.MoveLeft();
#if MOTORS_STEP_MODE
    delay(MOTORS_STEP_TIMEOUT_MS);
    wheels.Stop();
#endif
}
void D(void) {
    wheels.MoveRight();
#if MOTORS_STEP_MODE
    delay(MOTORS_STEP_TIMEOUT_MS);
    wheels.Stop();
#endif
}
void Shiver(void) {
    for (size_t i = 0; i < CONFIG_SHIVERS; i++)
    {
        wheels.MoveRight();
        delay(CONFIG_SHIVER_PERIOD_MS/2);
        wheels.MoveLeft();
        delay(CONFIG_SHIVER_PERIOD_MS/2);
    }
    Stop();
}

void Stop(void) {
    wheels.Stop();
}
