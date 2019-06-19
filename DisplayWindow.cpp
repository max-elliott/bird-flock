/* DisplayWindow.cpp
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * .cpp file for DisplayWindow, the window used to visually display the flock
 * as the simulation runs. Uses QT GUI libraries.
 */
#include "DisplayWindow.h"
#include "ui_DisplayWindow.h"
#include "Bird.h"
#include "Flock.h"
#include <cstdlib>
#include <QPainter>
#include <QTimer>
#include <QPen>
#include <QPoint>
#include <QSize>
#include <QPolygon>
#include <iostream>

/* Constructor. Sets up the window, intialises the data members, creates a timer
 * that calls the classes update method every 20ms. The update method comes from
 * the QWidget class, which DisplayWindow inherits from.
 */
DisplayWindow::DisplayWindow(Flock* flock, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);
    setStyleSheet("background-color:whit"
                  " e");
    setAutoFillBackground(false);
    setWindowTitle(std::string("Bird Flock Display").c_str());

    fFlock = flock;
    fPause = false;

    //Timers and connect explained in MainWindow.cpp constructor
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update())); //update() calls the paintEvent slot, so the Flock is redrawn every 20ms
    timer->start(20);
    show();

}

//Destructor
DisplayWindow::~DisplayWindow()
{
    delete ui;
}

/* paintEvent slot for DisplayWindow. This is how all flockObjects are drawn, and its is called every 20ms
 * by the QTimer created in the DisplayWindow constructor. It uses the QPainter class defined by the Qt
 * libraries.
 */
void DisplayWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);//new painter
    QPen pen(Qt::green);//new pen, used to define the line thickness/colour when drawing
    painter.setPen(pen);//set the painter's pen to the one declared above

    //loop to paint all birds in flock
    for(int i=0; i < fFlock->getBirds()->size(); i++){

        //get all data members needed to draw the bird.
        int x = (int)fFlock->getBirds()->at(i)->getXPos();
        int y = (int)fFlock->getBirds()->at(i)->getYPos();
        double heading = fFlock->getBirds()->at(i)->getHeading();
        std::string colour = fFlock->getBirds()->at(i)->getColour();

        //creates an isosceles triangle around the bird's position, using the heading to rotate in the right direction

        QPolygon shape;
        shape << QPoint(x+8*cos(heading),y+8*sin(heading)) <<
                QPoint(x-4*sin(heading), y+4*cos(heading)) <<
                QPoint(x+4*sin(heading), y-4*cos(heading)) <<
                QPoint(x+8*cos(heading),y+8*sin(heading));

        //setting pen colour based on bird colour
        if(colour.compare("blue")==0){
            pen.setColor(Qt::blue);
            painter.setPen(pen);
        }
        else if(colour.compare("green")==0){
            pen.setColor(Qt::green);
            painter.setPen(pen);
        }
        else if(colour.compare("red")==0){
            pen.setColor(Qt::red);
            QColor c = Qt::red;
            painter.setPen(pen);
        }

        //draws the bird
        painter.drawPolyline(shape);
        //painter.drawPolygon(shape);
    }

    //loop to paint all obstacles
    pen.setColor(Qt::black);
    painter.setPen(pen);
    for(int i=0;i<fFlock->getObstacles()->size();i++){
        Obstacle* o = fFlock->getObstacles()->at(i);
        int radius = o->getRadius();
        painter.drawEllipse(QPoint(o->getXPos(),o->getYPos()),radius, radius);
    }

    //adds the pause label if fPause is true, to indicate the simulation is paused.
    ui->Pause_label->setVisible(fPause);


}

//When the window is resized, the pause label is repositioned so that it is always central
void DisplayWindow::resizeEvent(QResizeEvent *E){
    QSize size = E->size();
    ui->Pause_label->setGeometry(size.width()*0.5-50,size.height()*0.5-25,100,50);
}
