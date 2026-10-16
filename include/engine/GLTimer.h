#pragma once
class GLTimer {
public:
    double startTime = 0;
    static double now(); // Monotonic milliseconds, consistent across platforms.
    static void useFixedTime(double milliseconds);
    static void useRealTime();
};
