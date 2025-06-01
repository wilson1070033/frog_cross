#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <vector>
using namespace std;

class GameUtils {
public:
    int randBtw(int n1, int n2);
    vector<int> generateVehiclePositions(int laneWidth, int vehicleCount, int minGap);
};

#endif // GAMEUTILS_H