/* Obstacle.h
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * Header file for Obstacles. Generates instances of circular obstacles that Birds
 * must avoid. A Bird will die if it hits an obstacle. Inherites from FlockObject.
 */
#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "FlockObject.h"

class Obstacle : public FlockObject{

public:

    //Constructor
    Obstacle(TwoVector position, int radius);

    //Deconstructor
    virtual ~Obstacle();

    //getter and setter for data member
    inline int const getRadius()const{return fRadius;}
    inline void setRadius(int newVal){fRadius=newVal;}

private:
    int fRadius;//the radius of the obstacle
};

#endif // OBSTACLE_H
