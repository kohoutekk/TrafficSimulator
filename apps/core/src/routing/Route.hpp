#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <vector>

class Lane;

struct Route {
    std::vector<Lane *> lanes;
    float totalLength = 0;
};

#endif // ROUTE_HPP
