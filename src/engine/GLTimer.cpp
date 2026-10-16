#include "GLTimer.h"
#include <chrono>
namespace {
bool fixedTime = false;
double elapsed = 0;
}
double GLTimer::now() {
    if (fixedTime) return elapsed;
    using Clock = std::chrono::steady_clock;
    static const auto start = Clock::now();
    return std::chrono::duration<double, std::milli>(Clock::now()-start).count();
}
void GLTimer::useFixedTime(double milliseconds) { fixedTime = true; elapsed = milliseconds; }
void GLTimer::useRealTime() { fixedTime = false; }
