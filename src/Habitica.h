#pragma once

#include "Entity.h"
#include "ArduinoJson.h"
#include <vector>

class Daily {
    public:
        // Lots more is returned in the entity:
        // "sensor.habitica_NAME_dailys",
        // But I don't use it at all.
        
        std::string id;
        std::string text;
        bool completed;
        int streak;
        float value;
        float priority;

        Daily(std::string id, JsonDocument& json);
};

class Todo {
    public:
        std::string id;
        std::string text;
        std::string notes;
        float value;
        float priority;

        Todo(std::string id, JsonDocument& json);
};

class Habitica {
    public:
        std::string name;
        std::string playerClass;
        int level;

        float exp;
        float expToNextLvl;
        float gold;
        float health;
        float maxHealth;
        float mana;
        float maxMana;

        std::vector<Daily> dailies;
        // std::vector<Habit> habits;
        std::vector<Todo> todos;

        void Update(Entities& entities);

        // I'm not sure what else uses these ids,
        // but Dailies and Todos definitely do.
        static bool IsId(const std::string& id);
};

