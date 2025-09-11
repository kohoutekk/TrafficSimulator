#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Lane.hpp"

#include <vector>

class Lane;

enum class TurnType { Straight, Left, Right, UTurn };

struct VehicleDef {
    float length = 4.5f;        // m
    float width = 1.8f;         // m
    float maxSpeed = 30.0f;     // m/s (~108 km/h)
    float maxAccel = 3.0f;      // m/s^2
    float maxDecel = 6.0f;      // m/s^2 (braking)
    float speed = 0.0f;
    float position = 0.0f;
    float accel = 0.15f;
    bool obeyTrafficLights = true;
};

class Vehicle {
    friend class Simulation;

public:
    explicit Vehicle(const VehicleDef& def, Lane *lane) :
        m_lane(lane),
        m_length(def.length),
        m_width(def.width),
        m_position(def.position),
        m_speed(def.speed),
        m_acceleration(def.accel),
        m_maxAcceleration(def.maxAccel),
        m_maxDeceleration(def.maxDecel),
        m_maxSpeed(def.maxSpeed),
        m_obeyTrafficLights(def.obeyTrafficLights)
    {
        m_lane->addVehicle(this);
    }

    virtual ~Vehicle() {
        m_lane->removeVehicle(this);
    }


    void update(float dt);
    void setSpeed(float speed) { m_speed = std::min(m_maxSpeed, speed); }
    void setPosition(float position);
    //void setMaxAccel(float maxAccel);
    //void setMaxDecel(float maxDecel);
    //void setWidth(float width);
    //void setLength(float length);
    //void setAccel(float accel);
    Lane* currentLane() const { return m_lane; };
    Lane* nextLane() const {
        if (m_routeIterator == m_route.end() || (m_routeIterator + 1) == m_route.end()) {
            return nullptr;
        }
        return *(m_routeIterator + 1);
    }
    float position() const { return m_position; }
    float length() const { return m_length; }

    // For sorting by position
    bool operator<(const Vehicle &v) const {
        return m_position < v.position();
    }

private:
    void changeLane(Lane *lane);

    void setRoute(std::vector<Lane *>&& route);

    Vehicle* findNearestVehicleAhead(float position) const;

    Lane *m_lane;
    std::vector<Lane *> m_route;
    std::vector<Lane *>::iterator m_routeIterator;
    bool m_obeyTrafficLights;
    float m_maxAcceleration;
    float m_maxDeceleration;
    float m_maxSpeed;
    float m_length;
    float m_width;
    float m_position;      // meters along lane
    float m_speed;         // m/s
    float m_acceleration;  // m/s^2
};

#endif // VEHICLE_HPP
