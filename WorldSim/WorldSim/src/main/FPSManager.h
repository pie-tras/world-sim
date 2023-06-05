#pragma once

#include "Global.h"
#include "game/GameStateManager.h"
#include "util/Timer.h"

class FPSManager {
    const long long ONE_SECOND = 1000000LL;

    int fps_cap;
    bool print_fps;

    int frames;
    Timer cycleTimer, fpsTimer;
    long long unprocessed, frameTime;
public:
    FPSManager(int fps_cap, bool print_fps);
    void runCycle(void (*cycle)(GameStateManager* gsm, GLFWwindow* window), GameStateManager* gsm, GLFWwindow* window);
};