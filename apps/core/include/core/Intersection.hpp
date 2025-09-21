#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <vector>

class Lane;

struct Position {
    float x;
    float y;
};

class Intersection {
    friend class Simulation;
    friend class Lane;

public:
    Intersection(float x, float y) : position(x, y) {}

    Position position;

    const std::vector<Lane*>& incomingLanes() const { return _incomingLanes; }
    const std::vector<Lane*>& outgoingLanes() const { return _outgoingLanes; }

private:
    std::vector<Lane *> _outgoingLanes;
    std::vector<Lane *> _incomingLanes;
};

#endif // INTERSECTION_HPP
