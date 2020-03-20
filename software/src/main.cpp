#include "display.h"
#include "motors.h"
#include "printf.h"
#include "serial.h"
#include "rotsensors.h"
#include "i2c.h"
#include "controlcallback.h"
#include "common_config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>


void receiveEvent(int howMany) {
    cmd = Wire.read(); // receive byte as a character
    printf("[I2C] I received: %d", cmd);
}

void setup() {
#if DEBUG
    printf_init();
#endif
    serial_init();

    // i2c_init();
    // display_init();
    rotsensors_init();
    motors_greeting();
    Wire.begin(0x2a);                // join i2c bus with address #8
    Wire.onReceive(receiveEvent); // register event
};


void loop() {
    // display_poll();
    serial_poll();
    control_poll();
}
