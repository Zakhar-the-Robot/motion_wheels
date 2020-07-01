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

    i2c.Init(PLATFORM_I2C_ADDRESS, 4, (void *)i2c_reqEv, (void *)i2c_rcvEv);
    rotsensors_init();
    motors_greeting();
};


void loop() {
    serial_poll();
    control_poll();
    i2c.Print();
}
