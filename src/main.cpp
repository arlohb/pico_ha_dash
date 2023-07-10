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
auto statuses = CreateStatuses();

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

    CreateLcdChars(lcd);
}

void loop() {
    delay(1000);

    dbgln("");

    ha.UpdateEntities();
    auto lightsStatus = ha.entities["light.tz3210_ttkgurpb_ts0504b_light"].state;
    bool lightsOn = lightsStatus == "on";

    if (!lightsOn) {
        lcd.noBacklight();
        return;
    }

    lcd.backlight();

    int i = 0;
    for(Status& status : statuses) {
        bool isAlive = status.isAlive();

        lcd.setCursor(10 * (i / 4), i % 4);
        lcd.write(isAlive ? tick : cross);
        lcdp(lcd, " {}", status.name);

        i++;
    }

    auto haTime = ha.entities["sensor.time"].state;

    lcd.setCursor(10, 3);
    lcdp(lcd, "{}", haTime);
}

