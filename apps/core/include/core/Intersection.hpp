#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <vector>

class Lane;

struct Position {
    float x;
    float y;
};

class Intersection {
    // Only Simulation can create and mutate
    friend class Simulation;

public:
    Position position;
    const std::vector<Lane*>& incomingLanes() const { return m_incomingLanes; }
    const std::vector<Lane*>& outgoingLanes() const { return m_outgoingLanes; }

private:
    Intersection(float x, float y) : position(x, y) {}

    std::vector<Lane *> m_incomingLanes;
    std::vector<Lane *> m_outgoingLanes;
};

#endif // INTERSECTION_HPP
