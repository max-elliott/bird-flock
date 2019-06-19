/* MainWindow.h
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * Header file for MainWindow, the main GUI window that contains all the controls
 * for the simulation. FlockObjects are added, removed, and altered using controls
 * in this window. The simulation can be paaused and reset here.
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Flock.h>
#include "DisplayWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //method that resets the simulation back to the original settings
    void reset();

//Slots used when widgets in the user interface are changed.
private slots:

    //Method that is called every 20ms to advance the simulation
    void simulate();

    void on_ResetButton_clicked();

    //slots to respond to controls being changed for blue Birds
    void on_B_Count_Box_valueChanged(int newCount);
    void on_B_Speed_Slider_sliderMoved(int position);
    void on_B_Sep_Slider_sliderMoved(int position);
    void on_B_Det_Slider_sliderMoved(int position);

    //slots to respond to controls being changed for green Birds
    void on_G_Count_Box_valueChanged(int newCount);
    void on_G_Speed_Slider_sliderMoved(int position);
    void on_G_Sep_Slider_sliderMoved(int position);
    void on_G_Det_Slider_sliderMoved(int position);

    //slots to respond to controls being changed for Predators
    void on_R_Count_Box_valueChanged(int newCount);
    void on_R_Speed_Slider_sliderMoved(int position);
    void on_R_Sep_Slider_sliderMoved(int position);
    void on_R_Det_Slider_sliderMoved(int position);
    void on_R_Hunger_Slider_sliderMoved(int position);

    //slots to respond to controls being changed for Obstacles
    void on_Obs_Count_Box_valueChanged(int newCount);
    void on_Obs_Radius_Slider_sliderMoved(int position);

    //slot to respond to the pause buttong being pressed
    void on_Pause_Button_clicked();

    //slots to respond the advanced settings being changed for blue Birds
    void on_B_Coh_Strength_Slider_sliderMoved(int position);
    void on_B_Ali_Strength_Slider_sliderMoved(int position);
    void on_B_Sep_Strength_Slider_sliderMoved(int position);
    void on_B_AvoidPred_Strength_Slider_sliderMoved(int position);

    //slots to respond to the advanced setting being changed for blue Birds
    void on_G_Coh_Strength_Slider_sliderMoved(int position);
    void on_G_Ali_Strength_Slider_sliderMoved(int position);
    void on_G_Sep_Strength_Slider_sliderMoved(int position);
    void on_G_AvoidPred_Strength_Slider_sliderMoved(int position);

private:
    Ui::MainWindow *ui; //instance of MainWindow.ui to generate the interface
    DisplayWindow *display;//Instance of DisplayWindow to visually show the representation

    /* An instance of Flock. It acts as an interface between the objects in the simulation
     * and the controls of the simulation in MainWindow. All FlockObjects are stored and
     * simulated in Flock, but the MainWindow tells the Flock when to run the simulation.
     * Whenever a control is changed in MainWindow, it calls the appropriate methods in
     * fFlock. fFlock is declared as a pointer so the DisplayWindow can read the data off
     * the stack when drawing the simulation, rather than having to retrive it from the
     * MainWindow's local stack every time.
     */
    Flock* fFlock;

    /* Dimensions of the DisplayWindow d. used to keep the Birds from leaving the screen
     * when the simulation is running. Initialised with initial dimensions of the window.*/
    int X_DIMENSION =1200;
    int Y_DIMENSION =800;

    //enumerators to show if the simulation is paused or running.
    enum{
        kRun,
        kPause
    };
    int fStatus;//uses the enumerators to declare the state of the simulation.
};

#endif // MAINWINDOW_H
