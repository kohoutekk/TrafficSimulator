#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Lane.hpp"

#include <vector>

using Route = std::vector<Lane *>;

enum class TurnType {
    Straight,
    Left,
    Right,
    UTurn
};

struct VehicleDef {
    float length = 4.5f;        // m
    float width = 1.8f;         // m
    float maxSpeed = 30.0f;     // m/s (~108 km/h)
    float maxAccel = 3.0f;      // m/s^2
    float maxDecel = 6.0f;      // m/s^2 (braking)
    float accel = 0.15f;        // m/s^2
};

class Vehicle {
    friend class Simulation;

public:
    void update(float dt);
    void setSpeed(float speed)              { _speed = std::min(_properties.maxSpeed, speed); }
    void setPosition(float position);
    const VehicleDef& properties() const    { return _properties; }
    float position() const                  { return _position; }
    float speed() const                     { return _speed; }
    Lane* currentLane() const               { return _currentLane; }
    Lane* nextLane() const {
        Lane* next = nullptr;
        if (_routePosition != _route.end() && (_routePosition + 1) != _route.end()) {
            next = *(_routePosition + 1);
        }
        return next;
    }

private:
    Vehicle(const VehicleDef& def, Lane *lane) : _properties(def), _currentLane(lane) {
        _currentLane->addVehicle(this);
    }
    ~Vehicle() {
        _currentLane->removeVehicle(this);
    }

    void setRoute(std::vector<Lane *>&& route);
    Vehicle* findNearestVehicleAhead(float position) const;

    float _position = 0;    // meters along lane
    float _speed = 0;       // m/s
    VehicleDef _properties;
    Route _route;
    Route::iterator _routePosition;
    Lane* _currentLane;
};

#endif // VEHICLE_HPP
