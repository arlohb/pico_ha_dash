#pragma once

#include "Octopi.h"
#include "Request.h"
#include <unordered_map>
#include <string>

class Entity {
    public:
        std::string entityId;
        std::string state;
        std::unordered_map<std::string, std::string> attrs;
};

class Ha {
    public:
        static const std::string host;
        static const std::string token;

        std::unordered_map<std::string, Entity> entities;

        Octopi octopi;

        Response GetStatus();
        void UpdateEntities();

        void CallService(std::string service, std::string body);

        const std::string timeId = "sensor.time";
        std::string Time();

        const std::string lightId = "light.tz3210_ttkgurpb_ts0504b_light";
        bool IsLightOn();
        void LightOn();
        void LightOff();
        void LightToggle();

        const std::string ths1Id = "sensor.ths_1_temperature";
        float ths1Temp();

        const std::string ths2Id = "sensor.ths_2_temperature";
        float ths2Temp();
};

