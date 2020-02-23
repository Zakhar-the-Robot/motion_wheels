#include <Arduino.h>
#include "motors.h"

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

static Connection_t connection[5] = {
    {.cmd_code = 119, .func = W},
    {.cmd_code = 115, .func = S},
    {.cmd_code = 97, .func = A},
    {.cmd_code = 100, .func = D},
    {.cmd_code = 32, .func = Stop},
};
Motors_dc2platform wheels(PINR1, PINR2, PINL1, PINL2);
ControlCallbacks Cc(connection, SIZE_ARR(connection));

void motors_greeting(void) {
    wheels.MoveBackward();
    delay(300);

    wheels.MoveForward();
    delay(300);
    wheels.Stop();
}

void W(void) {
    wheels.MoveForward();
    delay(25);
    wheels.Stop();
}
void S(void) {
    wheels.MoveBackward();
    delay(25);
    wheels.Stop();
}
void A(void) {
    wheels.MoveLeft();
    delay(25);
    wheels.Stop();
}
void D(void) {
    wheels.MoveRight();
    delay(25);
    wheels.Stop();
}
void Stop(void) {
    wheels.Stop();
}
