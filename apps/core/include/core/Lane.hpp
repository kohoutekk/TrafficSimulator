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
        return sqrt(pow(m_to->position.x - m_from->position.x, 2) + pow((m_to->position.y - m_from->position.y), 2));
    }

    Intersection* to() const { return m_to; }
    Intersection* from() const { return m_from; }

    //float toAngle() const { return m_toAngle; }
    //float fromAngle() const { return m_fromAngle; }

    const std::vector<Vehicle*>& vehicles() const { return m_vehicles; }
    const std::vector<TrafficLight*>& lights() const { return m_lights; }

private:
    Lane(const LaneDef &def, Intersection *from, Intersection *to)
        : m_from(from), m_to(to) {}

    void addVehicle(Vehicle *v) {
        // TODO: Avoid duplicates
        m_vehicles.push_back(v);
        std::sort(m_vehicles.begin(), m_vehicles.end());
    }

    void removeVehicle(Vehicle *v) {
        std::erase_if(m_vehicles, [v](auto* vehicle) { return vehicle == v; });
    }


    Intersection* m_from;
    Intersection* m_to;
    std::vector<Vehicle *> m_vehicles;
    std::vector<TrafficLight *> m_lights;
};

#endif // LANE_HPP
