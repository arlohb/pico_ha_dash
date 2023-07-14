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
    
    dailies.clear();
    const auto& dailiesAttrs = entities["sensor.habitica_arlo_dailys"].attrs;
    
    for(const auto& [id, jsonString]: dailiesAttrs) {
        if (Habitica::IsId(id)) continue;

        StaticJsonDocument<1024> json;
        deserializeJson(json, jsonString);

        Daily daily(id, json);
        dailies.push_back(daily);
    }

    todos.clear();
    const auto& todosAttrs = entities["sensor.habitica_arlo_todos"].attrs;

    for(const auto& [id, jsonString]: todosAttrs) {
        if (Habitica::IsId(id)) continue;

        StaticJsonDocument<1024> json;
        deserializeJson(json, jsonString);

        Todo todo(id, json);
        todos.push_back(todo);
    }
}

bool Habitica::IsId(const std::string& id) {
    return id.length() != 37 && id[8] != '-';
}

