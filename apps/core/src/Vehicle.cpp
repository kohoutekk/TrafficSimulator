#include "core/Vehicle.hpp"
#include "core/Lane.hpp"
#include "core/Intersection.hpp"

Vehicle* Vehicle::changeLane(Lane *desiredlane) {
    if (desiredlane->vehicles().at(0)->nextLane() == desiredlane) {

    }
}

Vehicle* Vehicle::findNearestVehicleAhead(float position) const {
    Vehicle* nearest = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (auto* v : m_lane->vehicles()) {
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
    Vehicle* nearest = findNearestVehicleAhead(m_position);

    if (nearest) {
        float gap = nearest->position() - m_position - nearest->length();

        float safeDistance = m_length + std::max(4.0f, m_speed * 2.5f);

        if (gap < safeDistance) {
            // Too close → slow down
            setSpeed(std::max(0.0f, m_speed - m_acceleration * dt));
        } else {
            // Free road → accelerate to desired speed
            setSpeed(m_speed + m_acceleration * dt);
        }
    } else {
        // No vehicle ahead → drive normally
        setSpeed(m_speed + m_acceleration * dt);
    }

    // Update position
    setPosition(m_position + m_speed * dt);
}

void Vehicle::setPosition(float position) {
    while (position >= m_lane->length()) {

        if (m_routeIterator != m_route.end()) {
            // move to next lane
            float leftover = m_position - m_lane->length();
            m_lane->removeVehicle(this);
            m_lane = *(m_routeIterator++);
            m_lane->addVehicle(this);
            position = leftover;
        } else {
            // end of path
            position = m_lane->length();
            m_speed = 0;
            break;
        }
    }

    m_position = position;
}

void Vehicle::setRoute(std::vector<Lane*>&& route) {
    if (route.empty()) return;

    m_route = std::move(route);

    // If we are already located on a lane found in the route
    // start the iteration from the lane that follows
    // otherwise start from the beginning
    auto it = std::find(m_route.begin(), m_route.end(), m_lane);
    m_routeIterator = it != m_route.end() ? std::next(it) : m_route.begin();
}
