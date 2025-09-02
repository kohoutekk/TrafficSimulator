#ifndef ROAD_HPP
#define ROAD_HPP

#include "Lane.hpp"
#include <vector>
#include <string>

struct Road {
    std::string name;

    void addLane(Lane *lane) {
        _lanes.push_back(lane);
    }

    float length() const {
        float maxLen = 0;
        for (auto* lane : _lanes)
            if (lane->length() > maxLen) maxLen = lane->length();
        return maxLen;
    }

private:
    std::vector<Lane *> _lanes;
};

#endif // ROAD_HPP
