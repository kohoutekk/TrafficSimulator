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


QPointF unitVector(float angleDeg) {
    float rad = angleDeg * M_PI / 180.0;
    return QPointF(std::cos(rad), std::sin(rad));
}


void drawLane(QGraphicsScene& scene, const Lane* lane, int trackCount = 2) {
    QLineF line(lane->from()->position.x, lane->from()->position.y, lane->to()->position.x, lane->to()->position.y);

    QLineF normal = line.normalVector();
    normal.setLength(1.0);
    QPointF offsetVec = normal.p2() - normal.p1();

    for(int i = 0; i < trackCount; ++i) {
        float d = (i - (trackCount-1)/2.0f) * 5.0f;
        QLineF parallelLine = line.translated(offsetVec * d);
        scene.addLine(parallelLine, QPen(Qt::gray, 2));
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // === Setup Simulation ===
    Simulation sim;

    auto A = sim.addIntersection(0, 0);
    auto B = sim.addIntersection(200, 0);
    auto C = sim.addIntersection(200, 150);
    auto D = sim.addIntersection(0, 150);
    auto E = sim.addIntersection(100, 300);

    qDebug() << A << B << C << D;

    LaneDef laneDiagonal{250};
    LaneDef laneHorizontal{200};
    LaneDef laneVertical{150};

    auto AC = sim.addLane(laneDiagonal, A, C);
    auto CA = sim.addLane(laneDiagonal, C, A);
    auto AB = sim.addLane(laneHorizontal, A, B);
    auto BA = sim.addLane(laneHorizontal, B, A);
    auto BC = sim.addLane(laneVertical, B, C);
    auto CB = sim.addLane(laneVertical, C, B);
    auto CD = sim.addLane(laneHorizontal, C, D);
    auto DC = sim.addLane(laneHorizontal, D, C);
    auto DA = sim.addLane(laneVertical, D, A);
    auto AD = sim.addLane(laneVertical, A, D);
    auto CE = sim.addLane(laneDiagonal, C, E);
    auto EC = sim.addLane(laneDiagonal, E, C);
    auto ED = sim.addLane(laneDiagonal, E, D);
    auto DE = sim.addLane(laneDiagonal, D, E);

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


    for (auto& lane : {AB, BC, CD, DA, AC, DE, CE}) drawLane(scene, lane, 2);

    // Vehicle item
    QGraphicsEllipseItem* carItem = scene.addEllipse(-5, -5, 7, 7, QPen(Qt::red), QBrush(Qt::red));
    QGraphicsEllipseItem* carItem2 = scene.addEllipse(-5, -5, 7, 7, QPen(Qt::blue), QBrush(Qt::blue));
    QGraphicsEllipseItem* carItem3 = scene.addEllipse(-5, -5, 7, 7, QPen(Qt::yellow), QBrush(Qt::yellow));

    auto drawCar = [&](Vehicle *v, QGraphicsEllipseItem *item, float laneOffset = 0.0f) {
        auto* lane = v->currentLane();
        QPointF from(lane->from()->position.x, lane->from()->position.y);
        QPointF to(lane->to()->position.x, lane->to()->position.y);
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


