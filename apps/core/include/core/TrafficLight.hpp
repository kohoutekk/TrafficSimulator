#ifndef TRAFFICLIGHT_HPP
#define TRAFFICLIGHT_HPP

#include <vector>

class Lane;

enum class TrafficLightColor {
    Red,
    Yellow,
    Green
};

struct TrafficLightDef {
    TrafficLightColor currentColor = TrafficLightColor::Green;
    float timeToNextChange = 25;
    float redDuration = 30;
    float yellowDuration = 5;
    float greenDuration = 25;
};

class TrafficLight {
public:
    TrafficLight(const TrafficLightDef *def, const std::vector<Lane *> &lanes) {
        redDuration = def->redDuration;
        yellowDuration = def->yellowDuration;
        greenDuration = def->greenDuration;
        timeToNextChange = def->timeToNextChange;
        controlledLanes = std::move(lanes);
    }

    TrafficLightColor currentColor;
    float timeToNextChange;
    float redDuration;
    float yellowDuration;
    float greenDuration;

    std::vector<Lane *> controlledLanes;

    void update(float dt) {
        timeToNextChange -= dt;
        if (timeToNextChange <= 0.0f) {
            // simple cycle logic
            switch (currentColor) {
            case TrafficLightColor::Red:
                currentColor = TrafficLightColor::Green;
                timeToNextChange = greenDuration;
                break;
            case TrafficLightColor::Green:
                currentColor = TrafficLightColor::Yellow;
                timeToNextChange = yellowDuration;
                break;
            case TrafficLightColor::Yellow:
                currentColor = TrafficLightColor::Red;
                timeToNextChange = redDuration;
                break;
            }
        }
    }


};

#endif // TRAFFICLIGHT_HPP
