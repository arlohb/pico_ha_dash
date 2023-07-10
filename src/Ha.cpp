#include "Ha.h"

#include <fmt/core.h>
#include <ArduinoJson.h>
#include "Utils.h"
#include "secrets.h"

namespace Ha {

const std::string Ha::host = secrets::haHost;
const std::string Ha::token = secrets::haToken;

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

Response Ha::GetStatus() {
    Request req("");
    return req.Get();
}

void Ha::UpdateEntities() {
    Request req("states");
    Response response = req.Get();

    entities.clear();

    for(const JsonVariant& obj : response.json.as<JsonArray>()) {
        Entity entity;

        entity.entityId = obj["entity_id"].as<JsonString>().c_str();
        entity.state = obj["state"].as<JsonString>().c_str();

        entities[entity.entityId] = entity;
    }

    response.json.clear();
}

void Ha::CallService(std::string service, std::string body) {
    Request req(fmt::format("services/{}", service));
    Response _ = req.Post(body);
}

}

