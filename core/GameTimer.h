#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <chrono>
using namespace std;
using namespace std::chrono;

class GameTimer {
private:
    high_resolution_clock::time_point lastTime;
    double deltaTime;

public:
    GameTimer();
    double update();
    double getDeltaTime() const;
};

#endif // GAMETIMER_H