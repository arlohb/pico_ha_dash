#include "Ha.h"

#include <ArduinoJson.h>
#include "Utils.h"
#include "secrets.h"

const std::string Ha::host = secrets::haHost;
const std::string Ha::token = secrets::haToken;

Response Ha::GetStatus() {
    Request req("");
    return req.Get(Filters::none);
}

void Ha::UpdateEntities() {
    Request req("states");
    Response response = req.Get(Filters::entities);

    entities.clear();

    for(const JsonVariant& obj : response.json.as<JsonArray>()) {
        Entity entity;

        entity.entityId = obj["entity_id"].as<JsonString>().c_str();
        entity.state = obj["state"].as<JsonString>().c_str();

        for(const JsonPair& attr : obj["attributes"].as<JsonObject>()) {
            const char* key = attr.key().c_str();
            const std::string value = attr.value().as<std::string>();

            entity.attrs[key] = value;
        }

        entities[entity.entityId] = entity;
    }

    response.json.clear();

    octopi.Update(entities);
    habitica.Update(entities);
}

void Ha::CallService(std::string service, std::string body) {
    Request req(fmt::format("services/{}", service));
    Response _ = req.Post(body);
}

std::string Ha::Time() {
    return entities[timeId].state;
}

bool Ha::IsLightOn() {
    std::string state = entities["light.tz3210_ttkgurpb_ts0504b_light"].state;
    return state == "on";
}

void Ha::LightOn() {
    CallService(
        "light/turn_on",
        fmt::format(R"({{ "entity_id": "{}" }})", lightId)
    );
}

void Ha::LightOff() {
    CallService(
        "light/turn_off",
        fmt::format(R"({{ "entity_id": "{}" }})", lightId)
    );
}

void Ha::LightToggle() {
    CallService(
        "light/toggle",
        fmt::format(R"({{ "entity_id": "{}" }})", lightId)
    );
}

float Ha::ths1Temp() {
    return std::stof(entities[ths1Id].state);
}

float Ha::ths2Temp() {
    return std::stof(entities[ths2Id].state);
}

