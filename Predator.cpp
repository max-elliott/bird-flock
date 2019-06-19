/* Predator.cpp
 * Author: Max Elliott 1434717
 * Created On: 2017-12-12
 *
 * .cpp file for Predator objects. Used to represent each predator in the flocking simulation. Essentially a Bird,
 * but with different update method, and the behaviour of chasing other birds and eating them. They have a hunger
 * indicating how many birds they can eat. Predators will be removed once they eat this amount of birds. Inherits
 * from Bird.
 */
#include "Predator.h"
#include <iostream>

//Constructor
Predator::Predator(TwoVector position, double maxSpeed, int heading, int separationDistance, int detectionDistance, int hunger) :
    Bird(position,maxSpeed,heading,separationDistance,detectionDistance,"red",0,0,0,0), fHunger(hunger){}

//destructor
Predator::~Predator(){}

/* update
 *
 * Modified update method for Predators. They do no flock with other predators, and so don't need
 * cohesion or alignment forces. They will still separate form nearby predators, and avoid walls
 * and obstacles.
 *
 */
void Predator::update(std::vector<Bird*>* flock, std::vector<Obstacle *> *obstacles, int xdim, int ydim){

    //find behavioural steering forces
    TwoVector walls = avoidWalls(xdim, ydim);//predators will avoid walls
    TwoVector sep = separation(flock);//predators will avoid getting too close to eachother
    TwoVector prey = hunt(flock);//predators will chase the closest non-predator bird
    TwoVector obs = avoidObstacles(obstacles);//predators will avoid obstacles

    //Weight behavioural forces
    walls = walls*4;
    prey = prey*3;
    obs = obs*4*getMaxSpeed();//Predators need to move away from obstacles quicker when moving faster, so its also scaled by fMaxSpeed

    //if out of bounds, die
    if(outOfBounds(xdim, ydim)) setIsDead(true);

    //applies the behavioural forces
    applyForce(sep + walls+prey+obs);
}

/* Finds the nearest bird in the predator's detection radius and generates a TwoVector (huntVector) that points towards it.
 * Once found, it calculates the 'steer', which is the force to be applied to change the predator's velocity correctly.
 */
TwoVector Predator::hunt(std::vector<Bird *> *flock){
    TwoVector huntVector;
    int nearestPreyDistance=10000;//set initially very high
    for(int j=0; j<flock->size(); j++){

        Bird* other = flock->at(j);
        TwoVector displacement = other->getPosition() - getPosition();
        double distance = displacement.mag();

        /*if the other bird is not itself & bird is closer than the predators det. radius & bird is closer than other birds checked
        * & bird is not another predator, set the huntVector to the displacement from the predator to the bird
        */
        if(distance > 0 && distance < getDetectionDistance() && distance < nearestPreyDistance && other->getColour().compare("red")!=0 && other->getColour().compare("yellow")!=0){

            huntVector = displacement;
            nearestPreyDistance = distance;

            //if the predator is close enough to the bird, kill the bird
            if(distance < 3){
                eat(other);
            }
        }
    }
    //if found a bird in detection radius, calculate steer vector and return it. Else return a zero vector
    if(nearestPreyDistance!=10000){

        huntVector = huntVector.Unit()*getMaxSpeed();
        TwoVector steer = huntVector - getVelocity();
        if(steer.mag()>getMaxForce()){steer = steer.Unit()*getMaxForce();}
        return steer;
    }
    else{

        return TwoVector();
    }
}

/* eat
 *
 * Called when a predator catches a bird (gets close enough to it). The Bird is set
 * to dead to be removed, and the predators hunger is decremented. If the hunger goes
 * to 0, the predator is also set to be removed.
 *
 * inputs:
 * - b : bird to be eaten. Bird passed as pointer so the Bird on the heap is killed, rather than a local copy
 *
 */
void Predator::eat(Bird* b){

    b->setIsDead(true); //kill bird
    setHunger(getHunger()-1); //reduce hunger

    //kill predator if full
    if(getHunger()<=0){
        setIsDead(true);
    }

}

