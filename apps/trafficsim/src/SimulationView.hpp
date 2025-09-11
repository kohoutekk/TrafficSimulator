#ifndef SIMULATIONVIEW_HPP
#define SIMULATIONVIEW_HPP

#include <QGraphicsView>

#include <core/Simulation.hpp>


class SimulationView : public QGraphicsView
{
public:
    SimulationView(Simulation *sim, QWidget *parent = nullptr)
        : m_sim(sim), QGraphicsView(parent)
    {
        setScene(&m_scene);

        for(auto &v : sim->vehicles) {
            scene->add
        }

    }

private:
    Simulation *m_sim;
    QGraphicsScene m_scene;
};

#endif // SIMULATIONVIEW_HPP
