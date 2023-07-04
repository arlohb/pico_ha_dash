#pragma once

#include <Arduino.h>
#include <fmt/core.h>
#include <hd44780.h>

static auto dbg = [](auto fmt, auto... args) {
    Serial.println(fmt::format(fmt, args...).c_str());
};

static auto lcdp = [](auto& lcd, auto fmt, auto... args) {
    lcd.print(fmt::format(fmt, args...).c_str());
};

