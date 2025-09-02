#include <QApplication>
#include <QTimer>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include "core/Simulation.hpp"
#include "core/Lane.hpp"
#include "core/Intersection.hpp"
#include "core/Vehicle.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // === Setup Simulation ===
    Simulation sim;

    auto A = sim.addIntersection();
    auto B = sim.addIntersection();
    auto C = sim.addIntersection();
    auto D = sim.addIntersection();
    auto E = sim.addIntersection();

    qDebug() << A << B << C << D;

    auto AC = sim.addLane(A, C, 250.0f);
    auto CA = sim.addLane(C, A, 250.0f);
    auto AB = sim.addLane(A, B, 200.0f);
    auto BA = sim.addLane(B, A, 200.0f);
    auto BC = sim.addLane(B, C, 150.0f);
    auto CB = sim.addLane(C, B, 150.0f);
    auto CD = sim.addLane(C, D, 200.0f);
    auto DC = sim.addLane(D, C, 200.0f);
    auto DA = sim.addLane(D, A, 150.0f);
    auto AD = sim.addLane(A, D, 150.0f);
    auto CE = sim.addLane(C, E, 150.0f);
    auto EC = sim.addLane(E, C, 150.0f);
    auto ED = sim.addLane(E, D, 150.0f);
    auto DE = sim.addLane(D, E, 150.0f);

    VehicleDef carDef{4.0f, 33.0f, 3.0f};
    Vehicle* car1 = sim.addVehicle(carDef, AC);
    sim.navigate(car1, E);

    carDef.accel = carDef.accel * 3;
    carDef.maxSpeed = carDef.maxSpeed * 2;
    Vehicle* car2 = sim.addVehicle(carDef, DA);
    sim.navigate(car2, E);

    Vehicle* car3 = sim.addVehicle(carDef, BA);
    sim.navigate(car3, E);


    // === Visualization ===
    QGraphicsScene scene;

    // Fake coordinates for intersections
    QMap<const Intersection*, QPointF> coords;
    coords[A] = QPointF(0, 0);
    coords[B] = QPointF(200, 0);
    coords[C] = QPointF(200, 150);
    coords[D] = QPointF(0, 150);
    coords[E] = QPointF(100, 250);

    // Draw intersections
    for (auto [inter, pos] : coords.asKeyValueRange()) {
        scene.addEllipse(pos.x() - 5, pos.y() - 5, 10, 10, QPen(Qt::black), QBrush(Qt::black));
    }

    // Draw lanes
    auto drawLane = [&](const Lane* lane, int trackCount = 2) {
        QPointF from = coords[lane->from()];
        QPointF to   = coords[lane->to()];
        QLineF line(from, to);

        QLineF normal = line.normalVector();
        normal.setLength(1.0);
        QPointF offsetVec = normal.p2() - normal.p1();

        for (int i = 0; i < trackCount; ++i) {
            float d = (i - (trackCount-1)/2.0f) * 5; // center tracks around main line
            QLineF parallelLine = line.translated(offsetVec * d);
            scene.addLine(parallelLine, QPen(Qt::gray, 2));
        }
    };
    for (auto& lane : {AB, BC, CD, DA, AC, DE, CE}) drawLane(lane, 2);

    // Vehicle item
    QGraphicsEllipseItem* carItem = scene.addEllipse(-5, -5, 10, 10, QPen(Qt::red), QBrush(Qt::red));
    QGraphicsEllipseItem* carItem2 = scene.addEllipse(-5, -5, 10, 10, QPen(Qt::blue), QBrush(Qt::blue));
    QGraphicsEllipseItem* carItem3 = scene.addEllipse(-5, -5, 10, 10, QPen(Qt::yellow), QBrush(Qt::yellow));

    auto drawCar = [&](Vehicle *v, QGraphicsEllipseItem *item, float laneOffset = 0.0f) {
        auto* lane = v->currentLane();
        QPointF from = coords[lane->from()];
        QPointF to   = coords[lane->to()];
        float t = v->position() / lane->length();
        QPointF pos = from + (to - from) * t;

        // Apply perpendicular offset for parallel track
        QLineF line(from, to);
        QLineF normal = line.normalVector();
        normal.setLength(1.0);
        QPointF offsetVec = normal.p2() - normal.p1();
        pos += offsetVec * laneOffset;

        item->setPos(pos);
    };

    // View
    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.show();

    // Timer to step simulation + update car position
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        sim.step(1.0f);

        // project vehicle position along lane

        drawCar(car1, carItem, -5);
        drawCar(car2, carItem2, -5);
        drawCar(car3, carItem3, -5);
        //drawCar(car4, carItem);
    });
    timer.start(100); // 100 ms



    return app.exec();
}
