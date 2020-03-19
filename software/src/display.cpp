#include "display.h"
#include "rotsensors.h"
#include "common_config.h"

Adafruit_SSD1306 display(-1);

void display_init(void) {
    display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS); // initialize with the I2C addr 0x3C (for the 128x32)
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Hello!");
    display.display();
    delay(500);
    display.clearDisplay();
    printf("[Display] Ready!\n");
}




void display_print(int l, int r) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Left: ");
    display.println(l);
    display.print("Right: ");
    display.println(r);
    display.display();
}

void display_poll(void)
{
    uint64_t now = millis();
    uint64_t next = millis() + TIMESTEP;
    if (next < now) {
        display_print(rs_l.GetCounter(), rs_r.GetCounter());
        next = millis() + TIMESTEP;
    }
}