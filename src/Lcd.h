#pragma once

#include <Arduino.h>
#include <hd44780.h>
#include <fmt/core.h>

#include <memory>

class Lcd {
    public:
        Lcd(std::unique_ptr<hd44780> lcd);

        template<typename... T>
        void Print(
            const int x,
            const int y,
            fmt::format_string<T...> fmt,
            T&&... args
        );

        void BacklightOn();
        void BacklightOff();

        const char cross = 0;
        const char tick = 1;

    private:
        std::unique_ptr<hd44780> lcd;

        const byte crossChar[8] = {
            0b00000,
            0b10001,
            0b01010,
            0b00100,
            0b00100,
            0b01010,
            0b10001,
            0b00000
        };

        const byte tickChar[8] = {
            0b00000,
            0b00000,
            0b01010,
            0b00000,
            0b10001,
            0b01110,
            0b00000,
            0b00000
        };
};

// This has to be defined in the header file
template<typename... T>
void Lcd::Print(
    const int x,
    const int y,
    fmt::format_string<T...> fmt,
    T&&... args
) {
    lcd->setCursor(x, y);
    lcd->print(fmt::format(fmt, args...).c_str());
}


