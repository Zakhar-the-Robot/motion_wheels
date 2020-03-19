#include "display.h"
#include "motors.h"
#include "printf.h"
#include "serial.h"
#include "rotsensors.h"
#include "i2c.h"
#include "controlcallback.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>

#define DEBUG

void setup() {

    serial_init();
    printf_init();
    i2c_init();
    // display_init();
    rotsensors_init();
    motors_greeting();
};

void loop() {
    // display_poll();
    serial_poll();
}
