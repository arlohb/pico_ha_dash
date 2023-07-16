#include "Io.h"

#include <Arduino.h>

Io::Io() {
    pinMode(lightBtn, INPUT_PULLUP);
}

bool Io::LightBtnPressed() {
    return !digitalRead(lightBtn);
}

