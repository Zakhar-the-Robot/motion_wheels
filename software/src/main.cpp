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
uint64_t step = 5000;
uint64_t now;
uint64_t next = millis() + step;
#define NO_CMD (-1)


void exec_cmd(int *cmd) {
    if (*cmd != NO_CMD) {
        Cc.Exec(*cmd);
        *cmd = NO_CMD;
    }
}

void receiveEvent(int howMany) {
    while (1 < Wire.available()) // loop through all but the last
    {
        cmd = Wire.read(); // receive byte as a character

        Serial.print("[I2C] I received: ");
        Serial.println(cmd, DEC);
        exec_cmd(&cmd);
    }
}


void serial_poll(void)
{
    if (Serial.available() > 0) {
        // read the incoming byte:
        cmd = Serial.read();

        // say what you got:
        Serial.print("[UART] I received: ");
        Serial.println(cmd, DEC);

        exec_cmd(&cmd);
    }
}

void serial_init(void)
{
    int baud = 9600;
    Serial.begin(baud);
    Serial.print("[UART] Ready! Baudrate: ");
    Serial.println(baud);
}

void i2c_init()
{
    int addr = 0x2a;
    Wire.begin(addr); // our address is 42 (or * = anything)
    Wire.onReceive(receiveEvent); // register event
    printf("[i2c] Ready! Address: %d\n", addr);

}

void display_poll(void)
{
    now = millis();
    if (next < now) {
        display_print(rs_l.GetCounter(), rs_r.GetCounter());
        next = millis() + step;
    }
}

void setup() {

    serial_init();
    printf_init();
    i2c_init();
    display_init();
    rotsensors_init();
    motors_greeting();
};


void loop() {
    // display_poll();
    serial_poll();

}