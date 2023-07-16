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

void Program::DrawPage() {
    switch (page) {
        case Page::Status: DrawStatus(); break;
        case Page::Todos: DrawTodos(); break;
        case Page::Player: DrawPlayer(); break;
        case Page::Page4: DrawEmptyPage(4); break;
        case Page::Page5: DrawEmptyPage(5); break;
        case Page::Page6: DrawEmptyPage(6); break;
        case Page::Page7: DrawEmptyPage(7); break;
        case Page::Page8: DrawEmptyPage(8); break;
    }
}

void Program::DrawStatus() {
    int i = 0;
    for(Status& status : statuses.statuses) {
        bool alive = status.alive;

        lcd.Print(0, i + 1, "{} {:<18}", alive ? lcd.tick : lcd.cross, status.name);

        i++;
    }

    lcd.Print(0, 0, "{}    {:.1f}   {:.1f}", ha.Time(), ha.ths1Temp(), ha.ths2Temp());
    
    if (ha.octopi.printStatus == "Printing")
        lcd.Print(10, 2, "{:02d}%  {}", ha.octopi.printProgress, ha.octopi.completionTime);

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

void Program::DrawTodos() {
    int y = 0;
    for (auto& todo : ha.habitica.todos) {
        lcd.Print(0, y++, "{:<20}", todo.text);
    }

    for (int yEmpty = y; yEmpty < 4; yEmpty++) {
        lcd.Print(0, yEmpty, "{:<20}", "");
    }
}

void Program::DrawPlayer() {
    auto DrawBar = [&](const int y, const char* label, const int val, const int max) {
        lcd.Print(0, y, "{} {:<3}[{:=<{}}{:<{}}]{:>3}",
            label,
            val,
            "", (int)(9 * val / max),
            "", (int)(9 - 9 * val / max),
            max
        );
    };

    DrawBar(0, "HP", ha.habitica.health, ha.habitica.maxHealth);
    DrawBar(1, "XP", ha.habitica.exp, ha.habitica.expToNextLvl);
    DrawBar(2, "MA", ha.habitica.mana, ha.habitica.maxMana);
    lcd.Print(0, 3, "{:<20}", "");
}

void Program::DrawEmptyPage(int i) {
    lcd.Print(0, 0, "Page {:<15}", i);
    lcd.Print(0, 1, "{:<20}", "");
    lcd.Print(0, 2, "{:<20}", "");
    lcd.Print(0, 3, "{:<20}", "");
}

void Program::Loop() {
    if (arduino::serialEventRun)
        arduino::serialEventRun();

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
    
    page = (Page)io.Selector();

    DrawPage();
}

