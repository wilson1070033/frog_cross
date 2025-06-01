#include "GameUtils.h"
#include <cstdlib>
#include <algorithm>
using namespace std;

int GameUtils::randBtw(int n1, int n2) {
    return n1 + rand() % (n2 - n1 + 1);
}

vector<int> GameUtils::generateVehiclePositions(int laneWidth, int vehicleCount, int minGap) {
    vector<int> positions;
    if(vehicleCount == 0) return positions;

    int totalGapSpace = (vehicleCount - 1) * minGap;
    int availableSpace = laneWidth - totalGapSpace;

    if(availableSpace < vehicleCount) {
        vehicleCount = max(1, availableSpace / (minGap + 1));
        totalGapSpace = (vehicleCount - 1) * minGap;
        availableSpace = laneWidth - totalGapSpace;
    }

    int firstPosition = randBtw(0, max(0, availableSpace - vehicleCount));
    positions.push_back(firstPosition);

    for(int i = 1; i < vehicleCount; i++) {
        int lastPosition = positions[i-1];
        int nextPosition = lastPosition + 1 + randBtw(minGap, minGap + 2);
        if(nextPosition >= laneWidth) {
            nextPosition = laneWidth - 1;
        }
        positions.push_back(nextPosition);
    }

    return positions;
}