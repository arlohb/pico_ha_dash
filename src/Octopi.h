#pragma once

#include "Entity.h"
#include <string>

class Octopi {
    public:
        std::string printFile;
        std::string printStatus;
        int printProgress;
        
        std::string completionTime;
        std::string printTimeTotal;
        std::string printTimeDone;
        std::string printTimeLeft;

        float socTemp;
        float bedTarget;
        float bedTemp;
        float toolTarget;
        float toolTemp;

        void Update(Entities& entities);
};

