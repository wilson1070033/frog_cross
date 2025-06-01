#include "GameTimer.h"
using namespace std;
using namespace std::chrono;

GameTimer::GameTimer() : deltaTime(0.0) {
    lastTime = high_resolution_clock::now();
}

double GameTimer::update() {
    auto currentTime = high_resolution_clock::now();
    deltaTime = duration<double, milli>(currentTime - lastTime).count();
    lastTime = currentTime;
    return deltaTime;
}

double GameTimer::getDeltaTime() const {
    return deltaTime;
}