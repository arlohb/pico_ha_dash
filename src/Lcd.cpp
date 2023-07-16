#include "Lcd.h"

#include <Wire.h>

Lcd::Lcd(std::unique_ptr<hd44780> lcd):
    lcd(std::move(lcd)) {}

void Lcd::Begin() {
    Wire.setSDA(12);
    Wire.setSCL(13);

    lcd->begin(20, 4);
    lcd->setBacklight(1);

    lcd->createChar(cross, crossChar);
    lcd->createChar(tick, tickChar);
}

void Lcd::BacklightOn() {
    lcd->backlight();
}

void Lcd::BacklightOff() {
    lcd->noBacklight();
}

