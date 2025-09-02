#ifndef ROUTEKEY_HPP
#define ROUTEKEY_HPP

#include <functional>

class Intersection;
class Lane;

struct RouteKey {
    Intersection* from;
    Intersection* to;

    bool operator==(const RouteKey& other) const {
        return from == other.from && to == other.to;
    }
};

struct RouteKeyHash {
    std::size_t operator()(const RouteKey& k) const noexcept {
        return std::hash<Intersection*>()(k.from) ^ (std::hash<Intersection*>()(k.to) << 1);
    }
};

#endif // ROUTEKEY_HPP
