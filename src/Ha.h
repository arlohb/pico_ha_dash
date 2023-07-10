#pragma once

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <unordered_map>
#include <string>

namespace Ha {

class Entity {
    public:
        std::string entityId;
        std::string state;
};

class Response {
    public:
        static const StaticJsonDocument<64> entityFilter;

        int code;
        ArduinoJson::StaticJsonDocument<32768> json;

        Response(int code):
            code(code) {}

        ~Response();
};

class Request {
    public:
        Request(std::string endpoint);
        ~Request();

        Response Get();
        Response Post(std::string& body);

    private:
        HTTPClient http;
};

class Ha {
    public:
        static const std::string host;
        static const std::string token;

        std::unordered_map<std::string, Entity> entities;

        Response GetStatus();
        void UpdateEntities();

        void CallService(std::string service, std::string body);
};

}

