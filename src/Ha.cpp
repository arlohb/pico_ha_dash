#include "Ha.h"

#include <fmt/core.h>
#include "Utils.h"
#include "secrets.h"

namespace Ha {

const std::string Ha::host = secrets::haHost;
const std::string Ha::token = secrets::haToken;

Request::Request(std::string endpoint) {
    http.begin(fmt::format("{}/api/{}", Ha::host, endpoint).c_str());

    http.addHeader(
        "Authorization",
        fmt::format("Bearer {}", Ha::token).c_str()
    );
    http.addHeader("Content-Type", "application/json");
}

Response Request::Get() {
    int code = http.GET();

    WiFiClient stream = http.getStream();

    Response response(code, "");

    while (stream.available()) {
        int data = stream.read();
        response.body += (char)data;
    }

    return response;
}

Request::~Request() {
    http.end();
}

Response Ha::GetStatus() {
    Request req("");
    return req.Get();
}

Response Ha::GetStates() {
    Request req("states");
    return req.Get();
}

}

