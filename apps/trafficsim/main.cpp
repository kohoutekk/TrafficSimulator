#include <QApplication>
#include <QTimer>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include "core/Simulation.hpp"
#include "src/MainWindow.hpp"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

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


    MainWindow window;
    window.show();

    return app.exec();
}
