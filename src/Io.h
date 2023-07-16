#pragma once

class Io {
    public:
        const int lightBtn = 28;
        const int selector = 26;

        Io();

        bool LightBtnPressed();

        // This is from 0 to 7
        int Selector();
};
