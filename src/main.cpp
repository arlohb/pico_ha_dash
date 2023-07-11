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

// All in milliseconds
const int updateTime = 10 * 1000;
int lastLoopTime = 0;
int updateTimer = updateTime;

int lightBtn = 28;

void SetupWiFi() {
    WiFi.begin(secrets::ssid.c_str(), secrets::password.c_str());

    while (WiFi.status() != WL_CONNECTED)
        delay(200);

    NTP.begin("pool.ntp.org", "time.nist.gov");
    NTP.waitSet();
}

void SetupLCD() {
    Wire.setSDA(12);
    Wire.setSCL(13);

    lcd.begin(20, 4);
    lcd.setBacklight(1);

    CreateLcdChars(lcd);
}

void setup() {
    Serial.begin(115200);
    SetupWiFi();
    SetupLCD();

    pinMode(lightBtn, INPUT_PULLUP);

    // This is the max value of 8.3 secs
    rp2040.wdt_begin(8.3*1000);
}

void loop() {
    delay(200);
    rp2040.wdt_reset();

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
        ha.LightToggle();
    }

    if (!ha.IsLightOn()) {
        lcd.noBacklight();
        return;
    } else
        lcd.backlight();

    int i = 0;
    for(Status& status : statuses.statuses) {
        bool alive = status.alive;

        lcd.setCursor(0, i + 1);
        lcd.write(alive ? tick : cross);
        lcdp(lcd, 2, i + 1, "{}", status.name);

        i++;
    }

    lcdp(lcd, 0, 0, "{}    {:.1f}   {:.1f}", ha.Time(), ha.ths1Temp(), ha.ths2Temp());
    
    if (ha.octopi.printStatus == "Printing")
        lcdp(lcd, 10, 2, "{:02d}%  {}", ha.octopi.printProgress, ha.octopi.completionTime);
    else
        lcdp(lcd, 10, 2, "          ");
}

