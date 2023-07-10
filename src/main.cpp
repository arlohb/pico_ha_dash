#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <ctime>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include <array>
#include <chrono>

#include "Ha.h"
#include "Status.h"
#include "Utils.h"
#include "secrets.h"

hd44780_I2Cexp lcd(39);

Ha::Ha ha;
Statuses statuses;

// Both in milliseconds
const int updateTime = 10 * 1000;
int lastLoopTime = 0;
int updateTimer = updateTime;

int lightBtn = 28;

void setup() {
    Serial.begin(115200);

    WiFi.begin(secrets::ssid.c_str(), secrets::password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    Wire.setSDA(12);
    Wire.setSCL(13);

    lcd.begin(20, 4);
    lcd.setBacklight(1);

    pinMode(lightBtn, INPUT_PULLUP);

    CreateLcdChars(lcd);
}

void loop() {
    delay(10);

    int now = millis();
    int delta = now - lastLoopTime;
    lastLoopTime = now;
    updateTimer += delta;
    if (updateTimer >= updateTime) {
        dbgln("\nUpdating");
        updateTimer = 0;

        ha.UpdateEntities();
        statuses.Update();
    }

    if (!digitalRead(lightBtn)) {
        dbgln("Btn pressed");
        ha.LightToggle();
    }

    bool lightsOn = ha.IsLightOn();

    if (!lightsOn) {
        lcd.noBacklight();
        return;
    } else
        lcd.backlight();

    int i = 0;
    for(Status& status : statuses.statuses) {
        bool alive = status.alive;

        lcd.setCursor(10 * (i / 4), i % 4);
        lcd.write(alive ? tick : cross);
        lcdp(lcd, " {}", status.name);

        i++;
    }

    lcd.setCursor(10, 3);
    lcdp(lcd, "{}", ha.entities["sensor.time"].state);
}

