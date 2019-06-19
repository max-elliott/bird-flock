/* DisplayWindow.h
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * Header file for DisplayWindow, the window used to visually display the flock
 * as the simulation runs. Uses QT GUI libraries.
 */
#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QWidget>
#include <Flock.h>
#include "QResizeEvent"

namespace Ui {
class DisplayWindow;
}

class DisplayWindow : public QWidget
{
    Q_OBJECT

public:

    //Constructors
    explicit DisplayWindow(QWidget *parent = 0);

    //Constructor that takes in a Flock pointer. This is the Flock it will draw on the window.
    explicit DisplayWindow(Flock* flock, QWidget *parent = 0);

    //Destructor
    ~DisplayWindow();

    //Simple method to toggle fPause
    inline void togglePause(){fPause = !fPause;}


private slots:

    //Paint event slot, used to draw the flock on the display screen
    void paintEvent(QPaintEvent *);

    //slot for when window is resized.
    void resizeEvent(QResizeEvent* E);

private:
    Ui::DisplayWindow *ui; //instance of DisplayWindow.ui to generate the interface
    Flock* fFlock;//pointer to Flock used in simulation
    bool fPause;
};

#endif // DISPLAYWINDOW_H
