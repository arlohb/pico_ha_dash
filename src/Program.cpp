#include "Program.h"

#include <Wire.h>
#include <WiFi.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <RP2040USB.h>
#include <pico/cyw43_arch.h>

#include <numeric>

#include "Utils.h"
#include "secrets.h"

void SetupWiFi() {
    cyw43_arch_init_with_country(CYW43_COUNTRY_UK);

    WiFi.begin(secrets::ssid.c_str(), secrets::password.c_str());

    while (WiFi.status() != WL_CONNECTED)
        delay(200);

    NTP.begin("pool.ntp.org", "time.nist.gov");
    NTP.waitSet();
}

Program::Program():
    lcd(Lcd(std::make_unique<hd44780_I2Cexp>(39)))
{
    rp2040.begin();
    __USBStart();

    rp2040.fifo.begin(1);

    Serial.begin(115200);

    SetupWiFi();

    // This is the max value of 8.3 secs
    rp2040.wdt_begin(8.3*1000);

    dbgln("Setup done");
}

void Program::MaybeUpdate() {
    int now = millis();
    updateTimer += now - lastLoopTime;
    lastLoopTime = now;

    if (updateTimer >= updateTime) {
        updateTimer = 0;
        Update();
    }
}

void Program::Update() {
    dbgln("\nUpdating");

    ha.UpdateEntities();
    rp2040.wdt_reset();
    statuses.Update();
    rp2040.wdt_reset();
}

void Program::Loop() {
    if (arduino::serialEventRun)
        arduino::serialEventRun();
    if (arduino::serialEvent1Run)
        arduino::serialEvent1Run();
    if (arduino::serialEvent2Run)
        arduino::serialEvent2Run();

    delay(200);
    rp2040.wdt_reset();

    MaybeUpdate();

    if (io.LightBtnPressed()) {
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

