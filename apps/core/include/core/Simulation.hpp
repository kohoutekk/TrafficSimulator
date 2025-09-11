#ifndef SIMULATION_H
#define SIMULATION_H

#include "Intersection.hpp"
#include "Lane.hpp"
#include "TrafficLight.hpp"
#include "Vehicle.hpp"

#include <vector>

class Simulation {
public:
    Intersection *addIntersection(float x, float y);

    Lane *addLane(const LaneDef &def, Intersection *from, Intersection *to);

    Vehicle *addVehicle(const VehicleDef& def, Lane *lane);

    TrafficLight *addTrafficLight(const TrafficLightDef *def, const std::vector<Lane *> &lanes);

    void removeLane(Lane *lane);

    void removeIntersection(Intersection* inter);

    void removeVehicle(Vehicle *vehicle);

    void step(float dt);

    void navigate(Vehicle *vehicle, const Intersection *to);



    std::vector<std::unique_ptr<TrafficLight>>  lights;
    std::vector<std::unique_ptr<Vehicle>>       vehicles;
    std::vector<std::unique_ptr<Lane>>          lanes;
    std::vector<std::unique_ptr<Intersection>>  intersections;

    static std::vector<Lane *> computeRoute(const Intersection *start, const Intersection *end);
};

#endif // SIMULATION_H
