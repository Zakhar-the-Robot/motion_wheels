#include "display.h"

Adafruit_SSD1306 display(-1);

void display_init(void) {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)
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