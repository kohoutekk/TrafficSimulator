#ifndef LANE_HPP
#define LANE_HPP

#include <vector>

class TrafficLight;
class Intersection;
class Vehicle;

class Lane {
    friend class Simulation;
    friend class Vehicle;

public:
    float length() const { return m_length; }

    Intersection* to() const { return m_to; }
    Intersection* from() const { return m_from; }

    const std::vector<Vehicle*>& vehicles() const { return m_vehicles; }
    const std::vector<TrafficLight*>& lights() const { return m_lights; }

private:
    Lane(Intersection *from, Intersection *to, float length)
        : m_from(from), m_to(to), m_length(length) {}

    void addVehicle(Vehicle *v) {
        m_vehicles.push_back(v);
    }
    void removeVehicle(Vehicle *v) {
        std::erase_if(m_vehicles, [v](auto* vehicle) { return v == vehicle; });
    }

    float m_length;
    Intersection* m_from;
    Intersection* m_to;
    std::vector<Vehicle *> m_vehicles;
    std::vector<TrafficLight *> m_lights;
};

#endif // LANE_HPP
