#pragma once

#include <string>
#include <unordered_map>
#include <type_traits>

class Entity {
    public:
        std::string entityId;
        std::string state;
        std::unordered_map<std::string, std::string> attrs;
};

class Entities: public std::unordered_map<std::string, Entity> {
    public:
        template<typename T>
        T Get(const std::string& key);

        template<typename T>
        T Take(const std::string& key);
};

template<typename T>
T Entities::Get(const std::string& key) {
    // By getting a reference,
    // the string is only copied if its returned,
    // as stoi and stof take refs
    std::string& state = (*this)[key].state;

    if constexpr (std::is_same_v<T, std::string>) {
        return state;
    } else if constexpr (std::is_same_v<T, int>) {
        return std::stoi(state);
    } else if constexpr (std::is_same_v<T, float>) {
        return std::stof(state);
    } else {
        return state;
    }
}

template<typename T>
T Entities::Take(const std::string& key) {
    T t = Get<T>(key);
    erase(key);
    return t;
}

