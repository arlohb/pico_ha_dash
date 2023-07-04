#include "Status.h"
#include "secrets.h"

#include <fmt/core.h>
#include <HTTPClient.h>

std::array<Status, 2> CreateStatuses() {
    return std::array<Status, 2> {
        Status {
            .name = "Octoprint",
            .isAlive = []() {
                HTTPClient http;

                http.begin(secrets::octopiHost.c_str());

                int response = http.GET();

                Serial.print("Octoprint: ");
                Serial.println(response);

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

                Serial.print("HA: ");
                Serial.println(response);

                return response == 200;
            },
        },
    };
}

