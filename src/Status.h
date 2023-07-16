#pragma once

#include <array>
#include <functional>
#include <string>

class Status {
    public:
        const std::string name;

        bool alive;

        const std::function<bool()> isAlive;
};

class Statuses {
    public:
        std::array<Status, 3> statuses;

        Statuses();

        void Update();
};

