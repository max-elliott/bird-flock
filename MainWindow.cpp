/* MainWindow.cpp
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * .cpp file for MainWindow, the main GUI window that contains all the controls
 * for the simulation. Uses QT GUI libraries. FlockObjects are added, removed,
 * and altered using controls in this window. The simulation can be paaused and
 * reset here.
 *
 */
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Bird.h"
#include "Predator.h"
#include "Obstacle.h"
#include "Flock.h"
#include <cstdlib>
#include <QTimer>
#include <iostream>
#include "DisplayWindow.h"

/* Constructor of MainWindow. It sets up the main user interface, sets the range
 * of all sliders and controls in the window, initialises the Flock, creates and
 * displays the DisplayWindow, and then uses a QTimer to run the simulation every 20ms.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("background-color:white");
    setAutoFillBackground(false);
    setWindowTitle(std::string("Bird Flock Controls").c_str());


    srand(time(NULL));//generate seed used for random number generation

    fFlock = new Flock();//initialise fFlock
    fStatus = kRun; //Sets the simulation to run
    reset();// Calls reset method to initalise the Flock with 50 green and 50 blue Birds, with initial settings

    //Set the range of all sliders
    ui->B_Speed_Slider->setRange(0,8);
    ui->B_Sep_Slider->setRange(5,100);
    ui->B_Det_Slider->setRange(10,500);
    ui->G_Speed_Slider->setRange(0,8);
    ui->G_Sep_Slider->setRange(5,100);
    ui->G_Det_Slider->setRange(10,500);
    ui->R_Speed_Slider->setRange(0,8);
    ui->R_Sep_Slider->setRange(5,100);
    ui->R_Det_Slider->setRange(10,500);
    ui->R_Hunger_Slider->setRange(1,20);
    ui->B_Ali_Strength_Slider->setRange(0,50);
    ui->B_Coh_Strength_Slider->setRange(0,50);
    ui->B_Sep_Strength_Slider->setRange(0,50);
    ui->B_AvoidPred_Strength_Slider->setRange(0,100);
    ui->G_Ali_Strength_Slider->setRange(0,50);
    ui->G_Coh_Strength_Slider->setRange(0,50);
    ui->G_Sep_Strength_Slider->setRange(0,50);
    ui->G_AvoidPred_Strength_Slider->setRange(0,100);

    //Initialise the display window and show it. Get initial dimensions.
    display = new DisplayWindow(fFlock);
    display->show();

    /* Create a QTimer, an object that generates a timeOut() signal
     * every interval of time specified by the user
     */
    QTimer *timer = new QTimer(this);

    /*connect method, that connects a signal to a slot. this means
     * every time the timer generates a timeout signal, the simulate()
     * slot in this instance of MainWindow will be called
     */
    connect(timer, SIGNAL(timeout()), this, SLOT(simulate()));

    timer->start(20);//Start the timer, pinging at 20ms intervals

    show();
}

//Destructor
MainWindow::~MainWindow()
{
    delete ui;
}


/* Method to run the simulation, that is called every 20ms.
 *
 */
void MainWindow::simulate(){

    //update dimensions of display window
    Y_DIMENSION = display->height();
    X_DIMENSION = display->width();

    /* If the simulation is unpaused, update and move all birds in fFlock by calling the simulateFlock method
     * Dimensions are passed as arguments so the Flock can tell the Bird's where the walls of the simulation are.
     */
    if(fStatus == kRun){
        fFlock->simulateFlock(X_DIMENSION, Y_DIMENSION);

        //update the box count values as birds/obstacles may have been removed by the simulation
        ui->B_Count_Box->setValue(fFlock->getBlueCount());
        ui->G_Count_Box->setValue(fFlock->getGreenCount());
        ui->R_Count_Box->setValue(fFlock->getPredCount());
        ui->Obs_Count_Box->setValue(fFlock->getObstacleCount());
    }
}

//Slot for pressing the reset button. Simply calls the reset method below
void MainWindow::on_ResetButton_clicked()
{
    reset();
}

/*Method to initialise the flock, and to reset it when the reset button is pressed.
 *Removes all objects, sets all sliders to original values, and adds 50 blue and 50 green birds.
 */
void MainWindow::reset(){

    //empty the flock of all objects
    fFlock->clearFlock();


    //reset all sliders to original values. Green and blue birds have different initial parameters to produce slightly different behaviour.
    //blue parameters
    ui->B_Speed_Slider->setValue(4);
    ui->B_Sep_Slider->setValue(30);
    ui->B_Det_Slider->setValue(90);
    ui->B_Speed_Value->setText(QString::number(4));
    ui->B_Sep_Value->setText(QString::number(30));
    ui->B_Det_Value->setText(QString::number(90));

    //green parameters
    ui->G_Speed_Slider->setValue(3);
    ui->G_Sep_Slider->setValue(20);
    ui->G_Det_Slider->setValue(50);
    ui->G_Speed_Value->setText(QString::number(3));
    ui->G_Sep_Value->setText(QString::number(20));
    ui->G_Det_Value->setText(QString::number(50));

    //predator parameters
    ui->R_Speed_Slider->setValue(5);
    ui->R_Sep_Slider->setValue(50);
    ui->R_Det_Slider->setValue(200);
    ui->R_Hunger_Slider->setValue(5);
    ui->R_Speed_Value->setText(QString::number(5));
    ui->R_Sep_Value->setText(QString::number(50));
    ui->R_Det_Value->setText(QString::number(200));
    ui->R_Hunger_Value->setText(QString::number(5));

    //obstacle parameters
    ui->Obs_Radius_Slider->setValue(5);
    ui->Obs_Radius_Value->setText(QString::number(5));

    //blue force strength parameters
    ui->B_Sep_Strength_Slider->setValue(15);
    ui->B_Coh_Strength_Slider->setValue(6);
    ui->B_Ali_Strength_Slider->setValue(10);
    ui->B_AvoidPred_Strength_Slider->setValue(50);
    ui->B_Sep_Strength_Value->setText(QString::number(1.5));
    ui->B_Coh_Strength_Value->setText(QString::number(0.6));
    ui->B_Ali_Strength_Value->setText(QString::number(1));
    ui->B_AvoidPred_Strength_Value->setText(QString::number(5));

    //green force strength parameters
    ui->G_Sep_Strength_Slider->setValue(15);
    ui->G_Coh_Strength_Slider->setValue(10);
    ui->G_Ali_Strength_Slider->setValue(11);
    ui->G_AvoidPred_Strength_Slider->setValue(50);
    ui->G_Sep_Strength_Value->setText(QString::number(1.5));
    ui->G_Coh_Strength_Value->setText(QString::number(1));
    ui->G_Ali_Strength_Value->setText(QString::number(1.1));
    ui->G_AvoidPred_Strength_Value->setText(QString::number(5));



    //adds 50 green and 50 blue birds initially. No predators or obstacles.
    //Birds are spawned in random position within the current display dimensions.
    for(int i=0; i<50; i++){
        fFlock->addBird(new Bird(TwoVector(rand()%X_DIMENSION, rand()%Y_DIMENSION),4,rand()%360,30,90, "blue",
                                 ui->B_Sep_Strength_Slider->value()/10.,ui->B_Coh_Strength_Slider->value()/10.,
                                 ui->B_Ali_Strength_Slider->value()/10.,ui->B_AvoidPred_Strength_Slider->value()/10.));
    }
    for(int i=0; i<50; i++){
        fFlock->addBird(new Bird(TwoVector(rand()%X_DIMENSION, rand()%Y_DIMENSION),3,rand()%360,20,50, "green",
                                 ui->G_Sep_Strength_Slider->value()/10.,ui->G_Coh_Strength_Slider->value()/10.,
                                 ui->G_Ali_Strength_Slider->value()/10.,ui->G_AvoidPred_Strength_Slider->value()/10.));
    }
    fFlock->setBlueCount(50);
    fFlock->setGreenCount(50);
    fFlock->setPredCount(0);
    fFlock->setObstacleCount(0);
}

/* Slot for when the pause button is pressed. Simply toggles fStatus, changes the button text,
 * and toggles the pause image on the DisplayWindow. */
void MainWindow::on_Pause_Button_clicked()
{
    if(fStatus == kPause){ //if currently paused
        ui->Pause_Button->setText("Pause"); //change button text
        fStatus = kRun; //change enumerator state

    }
    else{ //if currently running
        ui->Pause_Button->setText("Run"); //change button text
        fStatus = kPause; //change enumerator state
    }
    display->togglePause(); //add or reomve 'paused' sign from the display.
}

/* Slot for when the blue Bird box count value is changed. It will add or remove blue Birds depending
 * on whether to value has gone up or down.
 */
void MainWindow::on_B_Count_Box_valueChanged(int newCount)
{
    int blueCount = fFlock->getBlueCount();//gets current number of blueBirds in the flock

    /* If new box count value is greater than current count, get the difference between the two and add that many Birds.
     * Else if the new value is lower, then set fIsDead to true for the appropriate amount of Birds, so they will be removed
     * by fFlock when simulate() is next called.
     */
    if(blueCount < newCount){
        for(int i=0; i< newCount-blueCount; i++){
            //adds bird at a random position and with current settings. Tries adding until 'added == true', indicating a Bird has been added.
            bool added=false;
            while(!added){
                added = fFlock->addBird(new Bird(TwoVector(rand()%X_DIMENSION,rand()%Y_DIMENSION),ui->B_Speed_Slider->value(),rand()%360,ui->B_Sep_Slider->value(),
                                        ui->B_Det_Slider->value(), "blue",ui->B_Sep_Strength_Slider->value()/10.,ui->B_Coh_Strength_Slider->value()/10.,
                                        ui->B_Ali_Strength_Slider->value()/10.,ui->B_AvoidPred_Strength_Slider->value()/10.));//Set to current values of sliders
            }
        }
    }
    else if(blueCount > newCount){
        /* Cycles through fBirds in fFlock, checking each bird colour. If its the correct colour, set to dead.
         * It does this until the right amount of Birds have been set to dead.*/

        int removeCount=0;//number of set to dead
        int i=0;//current index being checked in fBirds

        while(removeCount < blueCount-newCount && i < fFlock->getBirds()->size()){
            Bird* b = fFlock->getBirds()->at(i);
            if(b->getColour().compare("blue")==0){
                fFlock->getBirds()->at(i)->setIsDead(true);
                removeCount++;
            }
            i++;
        }
    }

}

//Slot for when blue speed slider is changed. Sets all blue birds maxSpeed to new value
void MainWindow::on_B_Speed_Slider_sliderMoved(int position)
{
    fFlock->changeMaxSpeed("blue", position);
    ui->B_Speed_Value->setText(QString::number(position));
}

//Slot for when blue separation distance slider is changed. Sets all blue birds separation distance to new value
void MainWindow::on_B_Sep_Slider_sliderMoved(int position)
{
    fFlock->changeSepDistance("blue", position);
    ui->B_Sep_Value->setText(QString::number(position));
}

//Slot for when blue detection distance slider is changed. Sets all blue birds detection distance to new value
void MainWindow::on_B_Det_Slider_sliderMoved(int position)
{
    fFlock->changeDetDistance("blue", position);
    ui->B_Det_Value->setText(QString::number(position));
}

/* Slot for when the green Bird box count value is changed. It will add or remove green Birds depending
 * on whether to value has gone up or down. Same as the blue Bird version, but with green Birds.
 */
void MainWindow::on_G_Count_Box_valueChanged(int newCount)
{
    int greenCount = fFlock->getGreenCount();
    //fFlock->setGreenCount(arg1);


    if(greenCount < newCount){
        for(int i=0; i< newCount-greenCount; i++){
            bool added=false;
            while(!added){
                added = fFlock->addBird(new Bird(TwoVector(rand()%X_DIMENSION, rand()%Y_DIMENSION),ui->G_Speed_Slider->value(),rand()%360,ui->G_Sep_Slider->value(),
                                     ui->G_Det_Slider->value(), "green",ui->G_Sep_Strength_Slider->value()/10.,ui->G_Coh_Strength_Slider->value()/10.,
                                                 ui->G_Ali_Strength_Slider->value()/10.,ui->G_AvoidPred_Strength_Slider->value()/10.));//Set to current values of sliders
            }
        }
    }
    else if(greenCount > newCount){
        int removeCount=0;
        int i=0;
        while(removeCount < greenCount-newCount && i < fFlock->getBirds()->size()){
            Bird* b = fFlock->getBirds()->at(i);
            if(b->getColour().compare("green")==0){
                b->setIsDead(true);
                removeCount++;
            }
            i++;
        }
    }

}

//Slot for when green speed slider is changed. Sets all green birds maxSpeed to new value
void MainWindow::on_G_Speed_Slider_sliderMoved(int position)
{
    fFlock->changeMaxSpeed("green", position);
    ui->G_Speed_Value->setText(QString::number(position));
}

//Slot for when green separation distance slider is changed. Sets all green birds separation distance to new value
void MainWindow::on_G_Sep_Slider_sliderMoved(int position)
{
    fFlock->changeSepDistance("green", position);
    ui->G_Sep_Value->setText(QString::number(position));
}

//Slot for when green detection distance slider is changed. Sets all green birds detection distance to new value
void MainWindow::on_G_Det_Slider_sliderMoved(int position)
{
    fFlock->changeDetDistance("green", position);
    ui->G_Det_Value->setText(QString::number(position));
}

/* Slot for when the Predator box count value is changed. It will add or remove Predators depending
 * on whether to value has gone up or down. Same as the blue Bird version, but with Predators.
 */
void MainWindow::on_R_Count_Box_valueChanged(int newCount)
{
    int predatorCount = fFlock->getPredCount();

    if(predatorCount < newCount){
        for(int i=0; i< newCount-predatorCount; i++){

            bool added=false;
            while(!added){
                added =fFlock->addBird(new Predator(TwoVector(rand()%X_DIMENSION, rand()%Y_DIMENSION),ui->R_Speed_Slider->value(),rand()%360,
                                              ui->R_Sep_Slider->value(),ui->R_Det_Slider->value(), ui->R_Hunger_Slider->value()));//Set to current values of sliders
            }
        }
    }
    else if(predatorCount > newCount){
        int removeCount=0;
        int i=0;
        while(removeCount < predatorCount-newCount && i < fFlock->getBirds()->size()){
            Bird* b = fFlock->getBirds()->at(i);
            if(b->getColour().compare("red")==0){
                b->setIsDead(true);
                removeCount++;
            }
            i++;
        }
    }
    //std::cout << fFlock->getPredCount() << std::endl;
}

//Slot for when predator speed slider is changed. Sets all predators maxSpeed to new value
void MainWindow::on_R_Speed_Slider_sliderMoved(int position)
{
    fFlock->changeMaxSpeed("red", position);
    ui->R_Speed_Value->setText(QString::number(position));
}

void MainWindow::on_R_Sep_Slider_sliderMoved(int position)
{
    fFlock->changeSepDistance("red", position);
    ui->R_Sep_Value->setText(QString::number(position));
}

//Slot for when predator detection distance slider is changed. Sets all predators detection distance to new value
void MainWindow::on_R_Det_Slider_sliderMoved(int position)
{
    fFlock->changeDetDistance("red", position);
    ui->R_Det_Value->setText(QString::number(position));
}

//Slot for when predator hunger slider is changed. Sets all predators hunger to new value
void MainWindow::on_R_Hunger_Slider_sliderMoved(int position)
{
    fFlock->changeHunger(position);
    ui->R_Hunger_Value->setText(QString::number(position));
}

/*Slot for when the obstacel box count value is changed. Will add or remove obstacles depending
 * on whether the new value is higher or lower than the obstacleCount. */
void MainWindow::on_Obs_Count_Box_valueChanged(int newCount)
{
    int ObstacleCount = fFlock->getObstacleCount();
    fFlock->setObstacleCount(newCount);


    if(ObstacleCount < newCount){
        for(int i=0; i< newCount-ObstacleCount; i++){
            //Add new obstacle in random position, but not near the walls, as that can cause a lot of Birds to hit it
            fFlock->addObstacle(new Obstacle(TwoVector(0.1*X_DIMENSION+rand()%((int)(0.8*X_DIMENSION)), 0.1*Y_DIMENSION+rand()%((int)(0.8*Y_DIMENSION))),ui->Obs_Radius_Slider->value()));
        }
    }
    else if(ObstacleCount > newCount){
        for(int i=0; i< ObstacleCount-newCount; i++){

            fFlock->getObstacles()->at(ObstacleCount-1-i)->setIsDead(true);
        }
    }

}


void MainWindow::on_Obs_Radius_Slider_sliderMoved(int position)
{
    fFlock->changeObstacleRadius(position);
    ui->Obs_Radius_Value->setText(QString::number(position));
}


// -------------------------------------------------------------------------------- //
// Slots for advanced controls sliders. All call the appropriate method in fFlock to change
// the correct data member in the correct-coloured bird.
// -------------------------------------------------------------------------------- //
void MainWindow::on_B_Coh_Strength_Slider_sliderMoved(int position)
{
    fFlock->changeCohesionStrength("blue", position/10.);//divided by 10 to allow increments of 0.1 to the strengths
    ui->B_Coh_Strength_Value->setText(QString::number(position/10.));
}

void MainWindow::on_B_Ali_Strength_Slider_sliderMoved(int position)
{
    fFlock->changeAlignmentStrength("blue", position/10.);
    ui->B_Ali_Strength_Value->setText(QString::number(position/10.));
}

void MainWindow::on_B_Sep_Strength_Slider_sliderMoved(int position)
{
    fFlock->changeSeparationStrength("blue", position/10.);
    ui->B_Sep_Strength_Value->setText(QString::number(position/10.));
}

void MainWindow::on_B_AvoidPred_Strength_Slider_sliderMoved(int position)
{
    fFlock->changeAvoidPredatorStrength("blue", position/10.);
    ui->B_AvoidPred_Strength_Value->setText(QString::number(position/10.));
}

void MainWindow::on_G_Coh_Strength_Slider_sliderMoved(int position)
{
    fFlock->changeCohesionStrength("green", position/10.);
    ui->G_Coh_Strength_Value->setText(QString::number(position/10.));
}

void MainWindow::on_G_Ali_Strength_Slider_sliderMoved(int position)
{
    fFlock->changeAlignmentStrength("green", position/10.);
    ui->G_Ali_Strength_Value->setText(QString::number(position/10.));
}

void MainWindow::on_G_Sep_Strength_Slider_sliderMoved(int position)
{
    fFlock->changeSeparationStrength("green", position/10.);
    ui->G_Sep_Strength_Value->setText(QString::number(position/10.));
}


void MainWindow::on_G_AvoidPred_Strength_Slider_sliderMoved(int position)
{
    fFlock->changeAvoidPredatorStrength("green", position/10.);
    ui->G_AvoidPred_Strength_Value->setText(QString::number(position/10.));
}
