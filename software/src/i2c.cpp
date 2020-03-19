#include "i2c.h"
#include "serial.h"
#include "common_config.h"


void receiveEvent(int howMany) {
    while (1 < Wire.available()) // loop through all but the last
    {
        cmd = Wire.read(); // receive byte as a character

        Serial.print("[I2C] I received: ");
        Serial.println(cmd, DEC);
        exec_cmd(&cmd);
    }
}

void i2c_init(void)
{
    Wire.begin(PLATFORM_I2C_ADDRESS); // our address is 42 (or * = anything)
    Wire.onReceive(receiveEvent); // register event
    printf("[i2c] Ready! Address: %d\n", PLATFORM_I2C_ADDRESS);
}