#include "core/Vehicle.hpp"
#include "core/Lane.hpp"
#include "core/Intersection.hpp"


Vehicle* Vehicle::findNearestVehicleAhead(float position) const {
    Vehicle* nearest = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (auto* v : currentLane()->vehicles()) {
        if (v == this) continue;
        float distance = v->position() - position;
        if (distance > 0 && distance < minDistance) {
            minDistance = distance;
            nearest = v;
        }
    }
    return nearest;
}

void Vehicle::update(float dt) {
    Vehicle *nearest = findNearestVehicleAhead(_position);

    if (nearest) {
        float gap = nearest->position() - _position - nearest->properties().length;
        float safeDistance = _properties.length + std::max(4.0f, _speed * 2.5f);

        if (gap < safeDistance) {
            // Too close, slow down
            setSpeed(std::max(0.0f, _speed - _properties.accel * dt));
        } else {
            // Free road, accelerate to desired speed
            setSpeed(_speed + _properties.accel * dt);
        }
    } else {
        // No vehicle ahead, drive normally
        setSpeed(_speed + _properties.accel * dt);
    }

    // Update position
    setPosition(_position + _speed * dt);
}

void Vehicle::setPosition(float position) {
    while (position >= _currentLane->length()) {

        if (_routePosition != _route.end()) {
            // move to next lane
            float leftover = position - _currentLane->length();
            _currentLane->removeVehicle(this);
            _currentLane = *(_routePosition++);
            _currentLane->addVehicle(this);
            position = leftover;
        } else {
            // end of path
            position = _currentLane->length();
            _speed = 0;
            break;
        }
    }

    _position = position;
}


void Vehicle::setRoute(std::vector<Lane*>&& route) {
    if (route.empty()) return;

    _route = std::move(route);

    // If we are already located on a lane found in the route
    // start the iterator from the lane that follows
    // otherwise start from the beginning
    auto it = std::find(_route.begin(), _route.end(), _currentLane);
    _routePosition = it != _route.end() ? std::next(it) : _route.begin();
}
