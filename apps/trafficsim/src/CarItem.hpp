#ifndef CARITEM_HPP
#define CARITEM_HPP

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QPen>
#include <QBrush>

#include <core/Vehicle.hpp>

class CarItem : public QGraphicsPolygonItem {
public:
    CarItem(const Vehicle *vehicle) {
        // Simple rectangular car body (top-down)
        // Centered around (0,0) for easier transforms
        QPolygonF body;
        body << QPointF(-vehicle->length()/2, -10/2)   // front-left
             << QPointF(vehicle->length()/2, -10/2)    // front-right
             << QPointF(vehicle->length()/2, 10/2)     // rear-right
             << QPointF(-vehicle->length()/2, 10/2);   // rear-left

        setPolygon(body);

        // Add styling
        setPen(QPen(Qt::black, 2));
        setBrush(QBrush(Qt::blue));
    }
};
#endif // CARITEM_HPP
