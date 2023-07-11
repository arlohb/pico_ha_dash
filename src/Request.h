#pragma once

#include <ArduinoJson.h>
#include <HTTPClient.h>

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

