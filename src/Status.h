#pragma once

#include <array>
#include <functional>
#include <string>

class Status {
    public:
        const std::string name;

        const std::function<bool()> isAlive;
};

std::array<Status, 8> CreateStatuses();

