#include "Request.h"

#include "Ha.h"
#include <fmt/core.h>

StaticJsonDocument<64> CreateEntityFilter() {
    StaticJsonDocument<64> filter;
    filter[0]["entity_id"] = true;
    filter[0]["state"] = true;
    // TODO use this.
    /* filter[0]["attributes"] = true; */

    return filter;
}
const StaticJsonDocument<64> Response::entityFilter = CreateEntityFilter();

Response::~Response() {
    json.clear();
}

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

    Response response(code);

    deserializeJson(
        response.json,
        http.getStream(),
        DeserializationOption::Filter(Response::entityFilter)
    );

    return response;
}

Response Request::Post(std::string& body) {
    int code = http.POST((const uint8_t*)body.c_str(), body.size());

    Response response(code);

    deserializeJson(
        response.json,
        http.getStream()
    );

    return response;
}

Request::~Request() {
    http.end();
}

