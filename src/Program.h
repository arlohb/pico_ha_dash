#pragma once

#include "Ha.h"
#include "Status.h"
#include "Lcd.h"
#include "Io.h"

enum class Page {
    Status,
    Todos,
    Player,
    Page4,
    Page5,
    Page6,
    Page7,
    Page8,
};

class Program {
    public:
        Program();

        void Loop();

    private:
        void MaybeUpdate();
        void Update();

        Page page = Page::Status;

        void DrawPage();
        void DrawStatus();
        void DrawTodos();
        void DrawPlayer();
        void DrawEmptyPage(int i);

        Io io;
        Lcd lcd;
        Ha ha;
        Statuses statuses;

        // All in milliseconds
        const int updateTime = 10 * 1000;
        int lastLoopTime = 0;
        int updateTimer = updateTime;
};

