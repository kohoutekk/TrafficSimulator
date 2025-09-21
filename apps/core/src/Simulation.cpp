#include "core/Simulation.hpp"
#include "core/Intersection.hpp"
#include "core/Lane.hpp"
#include "core/Vehicle.hpp"
#include "core/TrafficLight.hpp"
#include "pathfinding/pathfinding.hpp"

#include <vector>

Simulation::~Simulation() {
    for (auto ptr : lanes) delete ptr;
    for (auto ptr : vehicles) delete ptr;
    for (auto ptr : intersections) delete ptr;
}

void Simulation::step(float dt) {
    for (auto v : vehicles)
        v->update(dt);
}

void Simulation::navigate(Vehicle *vehicle, const Intersection *to) {
    vehicle->setRoute(computeRoute(vehicle->currentLane()->to(), to));
}

Vehicle *Simulation::addVehicle(const VehicleDef& def, Lane *lane) {
    auto vehicle = new Vehicle(def, lane);
    vehicles.push_back(vehicle);
    return vehicle;
}

Lane *Simulation::addLane(const LaneDef &def, Intersection *from, Intersection *to) {
    auto lane = new Lane(def, from, to);
    lanes.push_back(lane);
    return lane;
}

Intersection *Simulation::addIntersection(float x, float y) {
    auto inter = new Intersection(x, y);
    intersections.push_back(inter);
    return inter;
}

TrafficLight *Simulation::addTrafficLight(const TrafficLightDef *def, const std::vector<Lane *>& lanes) {
    auto light = new TrafficLight(def, lanes);
    lights.push_back(light);

    // Tell each lane that it’s controlled by this light
    for (auto lane : lanes)
        lane->m_lights.push_back(light);

    return light;
}

void Simulation::removeVehicle(Vehicle *vehicle) {
    if (std::erase(vehicles, vehicle))
        delete vehicle;
}

void Simulation::removeLane(Lane *lane) {
    if(!std::erase(lanes, lane)) return;

    // Remove all vehicles on the lane
    for (auto v : lane->vehicles()) {
        removeVehicle(v);
    }

    // Recalculate routes of all vehicles that contain the lane
    for (auto v : vehicles) {
        auto it = std::find(v->_route.begin(), v->_route.end(), lane);
        if (it != v->_route.end()) {
            auto dest = v->_route.back();
            // If we're removing the vehicle's destination node
            // we can just remove it from the current route
            if (dest == lane)
                v->_route.pop_back();
            else
                // TODO: only recompute route after the removed lane
                v->setRoute(computeRoute(v->currentLane()->to(), dest->to()));
        }
    }

    delete lane;
}

void Simulation::removeIntersection(Intersection* inter) {
    if (!std::erase(intersections, inter)) return;

    // Delete all lanes connected to this intersection
    std::erase_if(lanes, [this, inter](auto lane) {
        if (lane->from() == inter || lane->to() == inter) {
            removeLane(lane);
            return true;
        }
        return false;
    });

    delete inter;
}

std::vector<Lane *> Simulation::computeRoute(const Intersection *start, const Intersection *end) {
    return pathfinding::bfs<const Intersection, Lane *>(
        start,
        end,
        [](const Intersection *node) { return node->_outgoingLanes; },
        [](Lane *lane) { return lane->to(); }
    );
}
