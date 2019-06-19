/* Obstacle.h
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * .cpp file for Obstacles. Generates instances of circular obstacles that Birds
 * must avoid. A Bird will die if it hits an obstacle.
 */
#include "Obstacle.h"
#include "TwoVector.h"

//Constructor
Obstacle::Obstacle(TwoVector position, int radius) :
    FlockObject(position){
    fRadius = radius;
}

//Deconstructor
Obstacle::~Obstacle(){}
