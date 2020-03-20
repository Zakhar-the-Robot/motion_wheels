#include "i2c.h"
#include "serial.h"
#include "common_config.h"

void receiveI2cEvent(int howMany) {
    while (1 < Wire.available()) // loop through all but the last
    {
        cmd = Wire.read(); // receive byte as a character
        printf("[I2C] I received: %d", cmd);
    }
}

void i2c_init(void)
{
    Wire.begin(PLATFORM_I2C_ADDRESS); // our address is 42 (or * = anything)
    Wire.onReceive(receiveI2cEvent); // register event
    printf("[i2c] Ready! Address: %d\n", PLATFORM_I2C_ADDRESS);
}