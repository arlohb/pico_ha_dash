#include <range/v3/all.hpp>
namespace rg = ranges;
namespace rv = ranges::views;

#include "Habitica.h"
#include "Utils.h"

Daily::Daily(std::string id, JsonDocument& json):
    id(),
    text(json["text"].as<std::string>()),
    completed(json["completed"].as<bool>()),
    streak(json["streak"].as<int>()),
    value(json["value"].as<float>()),
    priority(json["priority"].as<float>()) {}

Todo::Todo(std::string id, JsonDocument& json):
    id(id),
    text(json["text"].as<std::string>()),
    notes(json["notes"].as<std::string>()),
    value(json["value"].as<float>()),
    priority(json["priority"].as<float>()) {}

void Habitica::Update(Entities& entities) {
    name = entities.Take<std::string>("sensor.habitica_arlo_name");
    playerClass = entities.Take<std::string>("sensor.habitica_arlo_class");
    level = entities.Take<int>("sensor.habitica_arlo_lvl");

    exp = entities.Take<float>("sensor.habitica_arlo_exp");
    expToNextLvl = entities.Take<float>("sensor.habitica_arlo_tonextlevel");
    gold = entities.Take<float>("sensor.habitica_arlo_gp");
    health = entities.Take<float>("sensor.habitica_arlo_hp");
    maxHealth = entities.Take<float>("sensor.habitica_arlo_maxhealth");
    mana = entities.Take<float>("sensor.habitica_arlo_mp");
    maxMana = entities.Take<float>("sensor.habitica_arlo_maxmp");

    dailies = entities["sensor.habitica_arlo_dailys"].attrs
        | rv::filter([](const auto& pair) {
            return Habitica::IsId(pair.first);
        })
        | rv::transform([](const auto& pair) {
            const auto& [id, jsonString] = pair;

            StaticJsonDocument<1024> json;
            deserializeJson(json, jsonString);

            return Daily(id, json);
        })
        | rg::to_vector;
    entities.erase("sensor.habitica_arlo_dailys");

    todos = entities["sensor.habitica_arlo_todos"].attrs
        | rv::filter([](const auto& pair) {
            return Habitica::IsId(pair.first);
        })
        | rv::transform([](const auto& pair) {
            const auto& [id, jsonString] = pair;

            StaticJsonDocument<1024> json;
            deserializeJson(json, jsonString);

            return Todo(id, json);
        })
        | rg::to_vector;
    entities.erase("sensor.habitica_arlo_todos");
}

bool Habitica::IsId(const std::string& id) {
    return (id.length() == 36) && (id[8] == '-');
}

