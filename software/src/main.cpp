#include "display.h"
#include "motors.h"
#include "printf.h"
#include "rotsensors.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#define DEBUG

extern ControlCallbacks Cc;
int cmd;

void setup() {
    Serial.begin(9600);
    Serial.write("Hello");
    printf_init();
    display_init();
    rotsensors_init();
    motors_greeting();
};
uint64_t step = 5000;
uint64_t now;
uint64_t next = millis() + step;

void loop() {
    now = millis();
    if (next < now)
    {
        display_print(rs_l.GetCounter(), rs_r.GetCounter());
        next = millis() + step;
    }
    if (Serial.available() > 0) {
        // read the incoming byte:
        cmd = Serial.read();

        // say what you got:
        Serial.print("I received: ");
        Serial.println(cmd, DEC);
        Cc.Exec(cmd);
    }
}