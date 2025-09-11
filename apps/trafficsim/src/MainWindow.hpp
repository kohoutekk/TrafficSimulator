#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QSplitter>
#include <QGraphicsView>
#include <QToolBox>
#include <QLabel>
#include <QHBoxLayout>

#include <core/Simulation.hpp>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Central widget
        QWidget *central = new QWidget(this);
        setCentralWidget(central);

        // Splitter (horizontal)
        QSplitter *splitter = new QSplitter(Qt::Horizontal, central);

        // Graphics view (left)
        QGraphicsView *graphicsView = new QGraphicsView(splitter);

        // ToolBox (right)
        QToolBox *toolBox = new QToolBox(splitter);
        toolBox->addItem(new QLabel("Tool 1"), "Page 1");
        toolBox->addItem(new QLabel("Tool 2"), "Page 2");

        // Put splitter as layout
        QHBoxLayout *layout = new QHBoxLayout(central);
        layout->addWidget(splitter);

        // Initial sizes (graphicsView larger than toolbox)
        splitter->setStretchFactor(0, 3); // graphicsView expands
        splitter->setStretchFactor(1, 1); // toolbox smaller

        resize(1000, 600);
    }
};

#endif // MAINWINDOW_HPP
