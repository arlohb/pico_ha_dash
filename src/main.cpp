#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include <array>
#include <chrono>
#include <numeric>

#include "Ha.h"
#include "Status.h"
#include "Utils.h"
#include "Lcd.h"
#include "secrets.h"

Lcd lcd(std::make_unique<hd44780_I2Cexp>(39));

Ha ha;
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

void setup() {
    Serial.begin(115200);
    SetupWiFi();

    lcd.Begin();

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
        rp2040.wdt_reset();
        statuses.Update();
        rp2040.wdt_reset();
    }

    if (!digitalRead(lightBtn)) {
        ha.LightToggle();
    }

    if (!ha.IsLightOn()) {
        lcd.BacklightOff();
        return;
    } else
        lcd.BacklightOn();

    int i = 0;
    for(Status& status : statuses.statuses) {
        bool alive = status.alive;

        lcd.Print(0, i + 1, "{} {}", alive ? lcd.tick : lcd.cross, status.name);

        i++;
    }

    lcd.Print(0, 0, "{}    {:.1f}   {:.1f}", ha.Time(), ha.ths1Temp(), ha.ths2Temp());
    
    if (ha.octopi.printStatus == "Printing")
        lcd.Print(10, 2, "{:02d}%  {}", ha.octopi.printProgress, ha.octopi.completionTime);
    else
        lcd.Print(10, 2, "          ");

    const int completed = std::accumulate(
        ha.habitica.dailies.begin(),
        ha.habitica.dailies.end(),
        0,
        [](int sum, Daily& daily){
            return sum + daily.completed;
        }
    );

    lcd.Print(17, 3, "{}/{}", completed, ha.habitica.dailies.size());
}

