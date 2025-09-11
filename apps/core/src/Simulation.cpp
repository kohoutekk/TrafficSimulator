#include "core/Simulation.hpp"
#include "core/Intersection.hpp"
#include "core/Lane.hpp"
#include "core/Vehicle.hpp"
#include "core/TrafficLight.hpp"
#include "utils.hpp"
#include "pathfinding/pathfinding.hpp"

#include <vector>


Intersection *Simulation::addIntersection(float x, float y) {
    auto inter = std::unique_ptr<Intersection>(new Intersection(x, y));
    auto raw = inter.get();
    intersections.push_back(std::move(inter));
    return raw;
}

Lane *Simulation::addLane(const LaneDef &def, Intersection *from, Intersection *to) {
    auto lane = std::unique_ptr<Lane>(new Lane(def, from, to));
    auto raw = lane.get();
    lanes.push_back(std::move(lane));

    from->m_outgoingLanes.push_back(raw);
    to->m_incomingLanes.push_back(raw);

    return raw;
}

Vehicle *Simulation::addVehicle(const VehicleDef& def, Lane *lane) {
    auto vehicle = std::make_unique<Vehicle>(def, lane);
    auto raw = vehicle.get();
    vehicles.push_back(std::move(vehicle));
    lane->addVehicle(raw);

    return raw;
}

TrafficLight *Simulation::addTrafficLight(const TrafficLightDef *def, const std::vector<Lane *> &lanes) {
    auto light = std::make_unique<TrafficLight>(def, lanes);
    auto raw = light.get();
    lights.push_back(std::move(light));

    // Tell each lane that it’s controlled by this light
    for (auto *lane : lanes)
        lane->m_lights.push_back(raw);

    return raw;
}

void Simulation::step(float dt) {
    for (auto& v : vehicles) v->update(dt);
    for (auto& tl : lights) tl->update(dt);
}

void Simulation::navigate(Vehicle *vehicle, const Intersection *to) {
    vehicle->setRoute(Simulation::computeRoute(vehicle->currentLane()->to(), to));
}

void Simulation::removeLane(Lane *l)
{
    if (!l) return;

    // Remove all the vehicles that were situated on that lane
    // as the ground was just pulled from under their wheels
    std::erase_if(
        vehicles,
        [l](const auto& v) {
            return v->currentLane() == l;
        }
    );

    // Recalculate vehicle routes affected by lane's removal
    for (auto& v : vehicles) {
        auto it = std::find(v->m_route.begin(), v->m_route.end(), l);
        if (it != v->m_route.end()) {
            auto dest = v->m_route.back();
            // If we're removing the vehicle's destination node
            // we can just remove it from the current route
            if (dest == l)
                v->m_route.pop_back();
            else
                // TODO: only recompute route after the removed lane
                v->setRoute(computeRoute(v->currentLane()->to(), dest->to()));

        }
    }

    // Remove all pointers to this lane from intersections
    for (auto& i : intersections) {
        EntityUtils::removeRef(l, i->m_incomingLanes);
        EntityUtils::removeRef(l, i->m_outgoingLanes);
    }

    // Remove lane from the owning container
    EntityUtils::destroy(l, lanes);
}

void Simulation::removeIntersection(Intersection *i)
{
    if (!i) return;

    // Remove all lanes connected to intersection
    auto& outgoing = i->m_outgoingLanes;
    while (!outgoing.empty()) {
        removeLane(outgoing.back());  // always remove last element
    }

    auto& incoming = i->m_incomingLanes;
    while (!incoming.empty()) {
        removeLane(incoming.back());  // always remove last element
    }

    // Remove intersection from the owning container
    EntityUtils::destroy(i, intersections);
}

void Simulation::removeVehicle(Vehicle *v)
{
    if (!v) return;

    EntityUtils::destroy(v, vehicles);
}


std::vector<Lane *> Simulation::computeRoute(const Intersection *start, const Intersection *end) {
    return pathfinding::bfs<const Intersection, Lane *>(
        start,
        end,
        [](const Intersection *node) { return node->m_outgoingLanes; },
        [](Lane *lane) { return lane->to(); }
    );
}

