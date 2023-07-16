#include "Lcd.h"

#include <Wire.h>

Lcd::Lcd(std::unique_ptr<hd44780> lcd):
    lcd(std::move(lcd))
{
    Wire.setSDA(12);
    Wire.setSCL(13);

    this->lcd->begin(20, 4);
    this->lcd->setBacklight(1);

    this->lcd->createChar(cross, crossChar);
    this->lcd->createChar(tick, tickChar);
}

void Lcd::BacklightOn() {
    lcd->backlight();
}

void Lcd::BacklightOff() {
    lcd->noBacklight();
}

