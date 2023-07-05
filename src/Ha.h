#pragma once

#include <HTTPClient.h>
#include <string>

namespace Ha {

class State {
    public:
        std::string entityId;
        // TODO Make a date time
        std::string lastChanged;
        // TODO Standardise somehow
        std::string state;
};

class Response {
    public:
        int code;
        std::string body;

        Response(int code, std::string body):
            code(code),
            body(body) {}
};

class Request {
    public:
        Request(std::string endpoint);
        ~Request();

        Response Get();

    private:
        HTTPClient http;
};

class Ha {
    public:
        static const std::string host;
        static const std::string token;

        Response GetStatus();
        Response GetStates();
};

}

