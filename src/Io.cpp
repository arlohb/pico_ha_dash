#include "Io.h"

#include <Arduino.h>
#include "Utils.h"

Io::Io() {
    pinMode(lightBtn, INPUT_PULLUP);
    pinMode(selector, INPUT);
}

bool Io::LightBtnPressed() {
    return !digitalRead(lightBtn);
}

int Io::Selector() {
    int analog = analogRead(selector);
    int selected = analog / (1024 / 8);
    return selected;
}

