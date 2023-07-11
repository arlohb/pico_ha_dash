#pragma once

#include <string>
#include <unordered_map>

class Entity {
    public:
        std::string entityId;
        std::string state;
        std::unordered_map<std::string, std::string> attrs;
};

typedef std::unordered_map<std::string, Entity> Entities;

