#include <Arduino.h>
#include <WiFi.h>

#include <array>

#include "Status.h"
#include "secrets.h"

std::array<Status, 2> statuses = CreateStatuses();

int t = 0;

void setup() {
    Serial.begin(115200);

    WiFi.begin(secrets::ssid.c_str(), secrets::password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    delay(2000);
}

void loop() {
    int i = 0;
    for(Status& status : statuses) {
        const std::string& s = status.name;

        bool isAlive = status.isAlive();

        i++;
    }

    t++;

    delay(1000);
}

