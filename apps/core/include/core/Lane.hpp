#ifndef LANE_HPP
#define LANE_HPP

#include "Intersection.hpp"

#include <cmath>
#include <vector>

class TrafficLight;
class Intersection;
class Vehicle;

struct LaneDef {
    float width = 100;
    float maxSpeed = 20;
};

class Lane {
    friend class Simulation;
    friend class Vehicle;

public:
    float length() const {
        return sqrt(pow(_to->position.x - _from->position.x, 2) + pow((_to->position.y - _from->position.y), 2));
    }

    Intersection* to() const { return _to; }
    Intersection* from() const { return _from; }

    const std::vector<Vehicle*>& vehicles() const { return m_vehicles; }
    const std::vector<TrafficLight*>& lights() const { return m_lights; }

private:
    Lane(const LaneDef &def, Intersection *from, Intersection *to) : _from(from), _to(to) {
        _from->_outgoingLanes.push_back(this);
        _to->_incomingLanes.push_back(this);
    }

    ~Lane() {
        std::erase(_from->_outgoingLanes, this);
        std::erase(_to->_incomingLanes, this);
    }

    void addVehicle(Vehicle *v) {
        m_vehicles.push_back(v);
    }

    void removeVehicle(Vehicle *v) {
        std::erase(m_vehicles, v);
    }

    Intersection* _from;
    Intersection* _to;
    std::vector<Vehicle *> m_vehicles;
    std::vector<TrafficLight *> m_lights;
};

#endif // LANE_HPP
