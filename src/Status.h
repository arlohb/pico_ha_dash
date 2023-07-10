#pragma once

#include "Arduino.h"
#include "hd44780.h"
#include <array>
#include <functional>
#include <string>

const int cross = 0;
const byte crossChar[] = {
  0b00000,
  0b10001,
  0b01010,
  0b00100,
  0b00100,
  0b01010,
  0b10001,
  0b00000
};

const int tick = 1;
const byte tickChar[] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

void CreateLcdChars(hd44780& lcd);

class Status {
    public:
        const std::string name;

        const std::function<bool()> isAlive;
};

std::array<Status, 7> CreateStatuses();

