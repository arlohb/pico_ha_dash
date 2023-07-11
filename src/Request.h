#pragma once

#include <ArduinoJson.h>
#include <HTTPClient.h>

typedef StaticJsonDocument<64> Filter;

class Filters {
    public:
        static const Filter none;
        static const Filter entities;
};

class Response {
    public:
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

        Response Get(Filter filter);
        Response Post(std::string& body);

    private:
        HTTPClient http;
};

