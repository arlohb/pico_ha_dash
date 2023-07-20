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
    name = entities["sensor.habitica_arlo_name"].state;
    playerClass = entities["sensor.habitica_arlo_class"].state;
    level = std::stoi(entities["sensor.habitica_arlo_lvl"].state);

    exp = std::stof(entities["sensor.habitica_arlo_exp"].state);
    expToNextLvl = std::stof(entities["sensor.habitica_arlo_tonextlevel"].state);
    gold = std::stof(entities["sensor.habitica_arlo_gp"].state);
    health = std::stof(entities["sensor.habitica_arlo_hp"].state);
    maxHealth = std::stof(entities["sensor.habitica_arlo_maxhealth"].state);
    mana = std::stof(entities["sensor.habitica_arlo_mp"].state);
    maxMana = std::stof(entities["sensor.habitica_arlo_maxmp"].state);

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
}

bool Habitica::IsId(const std::string& id) {
    return (id.length() == 36) && (id[8] == '-');
}

