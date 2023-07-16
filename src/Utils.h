#pragma once

#include <Arduino.h>
#include <fmt/core.h>
#include <hd44780.h>

template<typename... T>
static void dbg(fmt::format_string<T...> fmt, T&&... args) {
    Serial.print(fmt::format(fmt, args...).c_str());
};

template<typename... T>
static void dbgln(fmt::format_string<T...> fmt, T&&... args) {
    Serial.println(fmt::format(fmt, args...).c_str());
};

#define dbgval(expr) dbgln(#expr": {}", expr)

static void dbgHeap() {
    dbgln("Used {} out of {} bytes of the heap",
        rp2040.getUsedHeap(),
        rp2040.getTotalHeap()
    );
}

