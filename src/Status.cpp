#include "Status.h"
#include "Utils.h"
#include "secrets.h"

#include <fmt/core.h>
#include <WiFi.h>
#include <HTTPClient.h>

std::array<Status, 8> CreateStatuses() {
    return {
        Status {
            .name = "WiFi",
            .isAlive = []() {
                return WiFi.isConnected();
            },
        },
        Status {
            .name = "Google",
            .isAlive = []() {
                HTTPClient http;
                http.begin("http://www.google.com");
                int response = http.GET();
                http.end();

                auto error = std::string(http.errorToString(response).c_str());
                dbg("Google: {}, {}", response, error);

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
                dbg("Octopi: {}, {}", response, error);

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
                dbg("HA: {}, {}", response, error);

                return response == 200;
            },
        },
        Status {
            .name = "Demo 5",
            .isAlive = []() {
                return true;
            },
        },
        Status {
            .name = "Demo 6",
            .isAlive = []() {
                return true;
            },
        },
        Status {
            .name = "Demo 7",
            .isAlive = []() {
                return true;
            },
        },
        Status {
            .name = "Demo 8",
            .isAlive = []() {
                return true;
            },
        },
    };
}

