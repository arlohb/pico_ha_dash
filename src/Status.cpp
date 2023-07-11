#include "Status.h"
#include "Utils.h"
#include "secrets.h"

#include <fmt/core.h>
#include <WiFi.h>
#include <HTTPClient.h>

void CreateLcdChars(hd44780& lcd) {
    lcd.createChar(cross, crossChar);
    lcd.createChar(tick, tickChar);
}

Statuses::Statuses():
    statuses({
        Status {
            .name = "Wifi",
            .isAlive = []() {
                HTTPClient http;
                http.begin("http://www.google.com");
                int response = http.GET();
                http.end();

                auto error = std::string(http.errorToString(response).c_str());
                dbgln("Google: {}, {}", response, error);

                return response == 200;
            },
        },
        Status {
            .name = "Octopi",
            .isAlive = []() {
                HTTPClient http;
                http.begin(secrets::octopiHost.c_str());
                int response = http.GET();
                http.end();

                auto error = std::string(http.errorToString(response).c_str());
                dbgln("Octopi: {}, {}", response, error);

                return response == 200 || response == 302;
            },
        },
        Status {
            .name = "HA",
            .isAlive = []() {
                HTTPClient http;

                http.begin(fmt::format("{}/api/", secrets::haHost).c_str());
                http.addHeader(
                    "Authorization",
                    fmt::format("Bearer {}", secrets::haToken).c_str()
                );
                http.addHeader("Content-Type", "application/json");

                int response = http.GET();
                http.end();

                auto error = std::string(http.errorToString(response).c_str());
                dbgln("HA: {}, {}", response, error);

                return response == 200;
            },
        },
    }) {}

void Statuses::Update() {
    for(Status& status : statuses)
        status.alive = status.isAlive();
}

