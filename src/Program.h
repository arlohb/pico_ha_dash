#pragma once

#include "Ha.h"
#include "Status.h"
#include "Lcd.h"
#include "Io.h"

class Program {
    public:
        Program();

        void Loop();

    private:
        void MaybeUpdate();
        void Update();

        Io io;
        Lcd lcd;
        Ha ha;
        Statuses statuses;

        // All in milliseconds
        const int updateTime = 10 * 1000;
        int lastLoopTime = 0;
        int updateTimer = updateTime;
};

