/* Bird.cpp
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * .cpp file for Bird objects. Used to represent each bird in the flocking simulation. Each Bird has methods to apply
 * each of its behaviours, which are all called with it's update() method.  Inherits from FlockObject.
 */
#include "Bird.h"
#include <cmath>
#include <TwoVector.h>
#include <typeinfo>
#include <iostream>

/* Constructor
 * Initialises the data members of the Bird using the given arguments.
 * Initial fVelocity is calculated from fMaxSpeed and fHeading using trigonometry
 */
Bird::Bird(TwoVector pos, double maxSpeed, double heading, int sepDist, int detDist, std::string colour, double separationStrength, double cohesionStrength,
           double alignmentStrength, double avoidPredatorStrength):
           FlockObject(pos), fMaxSpeed(maxSpeed), fHeading(heading), fSeparationDistance(sepDist),fDetectionDistance(detDist), fColour(colour),
           fSeparationStrength(separationStrength), fCohesionStrength(cohesionStrength), fAlignmentStrength(alignmentStrength), fAvoidPredatorStrength(avoidPredatorStrength)
{
    fVelocity = TwoVector(fMaxSpeed*cos(heading*M_PI/180.),fMaxSpeed*sin(heading*M_PI/180.));
}

// Deconstructor
Bird::~Bird(){}

/* update
 *
 * Updates velocity of Bird.
 * Applies each behavioural method and gets the returned TwoVector from each. It then applies the weightings of the
 * behaviours, then uses them to change the Bird's velocity through the applyForce method. It also checks whether the
 * Bird is still in bounds, and kills the Bird if they are.
 *
 * inputs:
 * - flock: the flock of all birds
 * - obstacles: all obstacles in the simulation
 * - xdim: current x dimension of display window
 * - ydim: current ydimension of display window
 *
 */
void Bird::update(std::vector<Bird*>* flock, std::vector<Obstacle *> *obstacles, int xdim, int ydim){

    //Find force due to each behaviours
    TwoVector coh = cohesion(flock);//move towards average position of neighbouring birds
    TwoVector sep = separation(flock);//move away from neighbours that are too close
    TwoVector ali = alignment(flock);//align velocity with neighbours velocity
    TwoVector walls = avoidWalls(xdim, ydim);//move away from the edge of the screen
    TwoVector pred = avoidPredators(flock);//flee from predators
    TwoVector obs = avoidObstacles(obstacles);//avoid running into obstacles

    //apply the weightings to each force
    coh = coh*getCohesionstrength();
    sep = sep*getSeperationStrength();
    ali = ali*getAlignmentStrength();
    pred = pred*getAvoidPredatorStrength();
    walls = walls*5;
    obs = obs*1.5*fMaxSpeed;//Birds need to move away from obstacles quicker when moving faster, so its also scaled by fMaxSpeed

    //check out of bounds
    if(outOfBounds(xdim, ydim)) setIsDead(true);

    //apply the forces to update the velocity
    applyForce(coh + sep + ali + walls + pred + obs);
}


//--------------------------------- The three basic behaviours: cohesion, separation, alignment ---------------------------------//


/* Cohesion
 *
 * Behavioural method to move Bird towards the average position of its neighbours of the same colour
 *
 * inputs:
 * - flock: all other Birds
 *
 * return: TwoVector - a 'force' vector to steer the Bird towards the average position of neighbours
 */
TwoVector Bird::cohesion(std::vector<Bird* >* flock){

    TwoVector cohesionVector;//vector to hold sum of positions of neighbours
    int neighbourCount=0;//number of neighbours

    /* For each bird in the flock, checks if they're in detection range. If they are, and are the same
     * colour, add their position to the cohesion vector */
    for(int j=0; j<flock->size(); j++){

        //get distance and colour
        Bird* other = flock->at(j);
        TwoVector displacement = other->getPosition() - getPosition();
        double distance = displacement.mag();
        std::string otherColour = other->getColour();

        //if in range and same colour, add other's position to sum of positions
        if(distance > 0 && distance < fDetectionDistance && otherColour.compare(getColour())==0){
            cohesionVector += other->getPosition();
            neighbourCount++;
        }
    }

    //If there were neighbours, find the steering force to be returned. Else return a zero vector (no force)
    if(neighbourCount>0){
        cohesionVector = cohesionVector*(1./neighbourCount);//divide to find avg position

        //find the desired veolcity of the bird i.e. a vector from bird to avg position
        TwoVector desired = cohesionVector - getPosition();
        desired = desired.Unit()*fMaxSpeed; //scale the desired vector

        //find the steer i.e. a vector that takes the bird from current velocity to desired velocity
        TwoVector steer = desired - fVelocity;

        //limit the force to improve realism of movement
        if(steer.mag()>fMaxForce){steer = steer.Unit()*fMaxForce;}

        return steer;
    }
    else{
        return TwoVector(0,0);
    }
}

/* Separation
 *
 * Behavioural method to move Bird away from neighbours that are too close. All bird
 * colours repel each other.
 *
 * inputs:
 * - flock: all other Birds
 *
 * return: TwoVector - a 'force' vector to steer the Bird away from close neighbours
 */
TwoVector Bird::separation(std::vector<Bird* >* flock){
    TwoVector separationVector;//vector to hold the combined repulsive force of all close neighbours
    int neighbourCount=0;

    /* For each other bird in the flock, it checks if its within the separation distance
     * of the bird. If it is, it creates a repulsive force in the direction away from the
     * other bird, proportional to 1/distance.
     * */
    for(int j=0; j<flock->size(); j++){

        //get displacement and distance
        Bird* other = flock->at(j);
        TwoVector displacement = other->getPosition() - getPosition();
        double distance = displacement.mag();

        //if within separation distance, get repelled away (distance > 0 stops the bird repelling itself)
        if(distance > 0 && distance < fSeparationDistance){
            neighbourCount++;
            TwoVector repulsion = displacement.Unit()*(1/distance);
            separationVector -= repulsion;
        }
    }

    //If there were neighbours, find the steering force to be returned. Else return a zero vector (no force)
    if(neighbourCount>0){

        TwoVector desired = separationVector.Unit()*fMaxSpeed;//scale force

        //find the steer i.e. a vector that takes the bird from current velocity to desired velocity
        TwoVector steer = desired - fVelocity;

        //limit the force to improve realism of movement
        if(steer.mag()>fMaxForce){steer = steer.Unit()*fMaxForce;}

        return steer;
    }
    else{
        return TwoVector(0,0);
    }
}

/* Alignment
 *
 * Behavioural method to align Bird with average velocity of its neighbours of the same colour
 *
 * inputs:
 * - flock: all other Birds
 *
 * return: TwoVector - a 'force' vector to steer the Bird towards the correct velocity
 */
TwoVector Bird::alignment(std::vector<Bird* >* flock){
    TwoVector alignmentVector;//vector of sum of velocities of neighbours
    int neighbourCount=0;

    /* For each bird in the flock, checks if they're in detection range. If they are, and are the same
     * colour, add their velocity to the alignment vector */
    for(int j=0; j<flock->size(); j++){

        Bird* other = flock->at(j);
        TwoVector displacement = other->getPosition() - getPosition();
        double distance = displacement.mag();

            if(distance > 0 && distance < fSeparationDistance && getColour().compare(other->getColour())==0){
                neighbourCount++;
                alignmentVector += other->getVelocity();
            }
    }

    //If there were neighbours, find the steering force to be returned. Else return a zero vector (no force)
    if(neighbourCount>0){

        //scale the vector
        alignmentVector = alignmentVector.Unit()*fMaxSpeed;

        //find the steer i.e. a vector that takes the bird from current velocity to desired velocity
        TwoVector steer = alignmentVector - fVelocity;

        //limit the force to improve realism of movement
        if(steer.mag()>fMaxForce){steer = steer.Unit()*fMaxForce;}

        return steer;
    }
    else{
        return TwoVector(0,0);
    }
}

//----------------- other behaviours: avoidWalls, avoidPredators, avoidObstacles ----------------//

/* avoidWalls
 *
 * Method to repel birds away from the edges of the display if they get too close. This stop the
 * Birds leaving the display. It uses a repulsive force away form the edge, proportional to
 * 1/distance to wall. Note that birds can still sometimes leave the display at the highest speeds,
 * at which point they will be set to dead.
 *
 * inputs:
 * - xdim: current x dimension of the display window
 * - ydim: current y dimension of the display window
 *
 * returns: Twovector 'force' vector to push the Bird away from the wall
 * */
TwoVector Bird::avoidWalls(int xdim, int ydim){

        //get birds position
        double xPos = getPosition().x();
        double yPos = getPosition().y();
        TwoVector edgeRepulsion;

        /* If the Bird is near a wall, a repulsive force away from that wall
         * equal to 1/distance from that wall will be generated. A bird is
         * close to a wall if its distance to the wall is <10% of the
         * width/height of the display window.
         */
        if(yPos < ydim/10.){
            edgeRepulsion.SetY(1/yPos);
            }
        else if(yPos > 9*ydim/10.){
            edgeRepulsion.SetY(-1/(ydim-yPos));
        }
        if(xPos < xdim/10.){
            edgeRepulsion.SetX(1/xPos);
        }
        else if(xPos > 9*xdim/10.){
            edgeRepulsion.SetX(-1/(xdim-xPos));
        }

        return edgeRepulsion;
}

/* avoidPredators
 *
 * Behavioural method that causes Birds to flee from nearby Predators.
 *
 * inputs:
 * - flock: all other Birds
 *
 * return: TwoVector - a 'force' vector to steer the Bird away from nearby predators
 */
TwoVector Bird::avoidPredators(std::vector<Bird* >* flock){
    TwoVector avoidVector;//vector from bird to avg position of neighbours
    int predatorCount=0;

    /* Looks through flock and checks whether if each bird is a predator and within
     * detection distance. Creates a repulsive force like the seperate behaviour.
     * */
    for(int i=0; i<flock->size(); i++){

        Bird* other = flock->at(i);
        TwoVector displacement = other->getPosition() - getPosition();
        double distance = displacement.mag();

        //if a predator is in range, run from it
        if(distance < fDetectionDistance && other->getColour().compare("red")==0){
                predatorCount++;
                TwoVector repulsion = displacement.Unit()*(1/distance);
                avoidVector -= repulsion;
        }
    }
    //If there were neighbours, find the steering force to be returned. Else return a zero vector (no force)
    if(predatorCount>0){


        TwoVector desired = avoidVector.Unit()*fMaxSpeed;//scale force

        //find the steer i.e. a vector that takes the bird from current velocity to desired velocity
        TwoVector steer = desired - fVelocity;

        //limits force for more realistic movement
        if(steer.mag()>fMaxForce){steer = steer.Unit()*fMaxForce;}

        return steer;
    }
    else{
        return TwoVector(0,0);
    }
}

/* avoidObstacles
 *
 * Behavioural method that causes Birds to steer away from obstacles. If a bird is facing
 * an obstacle, and the obstacle is close enough, then the bird will veer to the side of the obstacle.
 *
 * inputs:
 * - obstacles: vector of all obstacles
 *
 * return: TwoVector - a 'force' vector to steer the Bird away from the obstacles
 */
TwoVector Bird::avoidObstacles(std::vector<Obstacle *> *obstacles){
    TwoVector avoidVector;//vector to store the steering force

    for(int i=0; i<obstacles->size(); i++){

        Obstacle* o = obstacles->at(i);
        double oRadius = o->getRadius();
        TwoVector displacement = o->getPosition() - getPosition();//vector between bird and centre of obstacle
        double distance = displacement.mag();

        //if bird ends up inside an obstacle, it dies
        if(distance < oRadius){
            setIsDead(true);
        }

        //make a vector in direction of the bird's velocity, with magnitude of distance
        TwoVector direction = getVelocity().Unit()*distance;

        //difference between the direciton vector and the vector between the bird and obstacle
        TwoVector facingObstacleCheck = direction - displacement;

        /*If the magnitude of this check is less than oRadius, then bird is facing the obstacle and
         *if close enough to have to worry about it. To point at which the bird must react is actually
         * set to 1.5*oRadius, so the Birds can react sooner, resulting in more realistic movement
         * and less Birds dying. */
        if(facingObstacleCheck.mag() <= 1.5*oRadius){
            avoidVector += facingObstacleCheck.Unit()*(1/(distance-oRadius)); //repulsive force away from obstacle
        }

    }

        return avoidVector;
}

/* applyForce
 *
 * Method that takes each force and uses it to update the velocity of the Bird. This is essentially
 * F=ma, but with m=1, so the forces become an acceleration.
 *
 */
void Bird::applyForce(TwoVector force){
    /*adds the behavioural forces to the velocity. If the acceleration is 0, then the bird will slightly
     accelerate in the direction of its velocity. */
    if(force.x()==0 and force.y()==0){
        setVelocity(getVelocity()*1.01);
    }
    else{
        setVelocity(getVelocity() + force);
    }

    //if the bird exceeds maxSpeed, it is limited to maxSpeed
    if(getVelocity().mag()>getMaxSpeed()){
        setVelocity(getVelocity().Unit()*getMaxSpeed());
    }

    //update heading
    changeHeading();

}

/* changeheading
 * Sets the heading of the Bird to its velocity. Called in applyForce after velocity has been updated.
 */
void Bird::changeHeading(){
    if(fVelocity.mag() != 0){
        //If moving in the positive x direction, use arctan as normal.
        //If moving in negative x direction, pi has to be added to the heading to get the right angle
        if(fVelocity.x()>=0){
            setHeading(atan(fVelocity.y()/fVelocity.x()));
        }
        else{
            setHeading(M_PI+atan(fVelocity.y()/fVelocity.x()));
        }
    }
}

/* move
 * Method to move the Bird by adding the veolocity to position.
 */
void Bird::move(){

    double newX = getPosition().x() +getVelocity().x();
    double newY = getPosition().y() +getVelocity().y();
    setXPos(newX);
    setYPos(newY);
}

/* outOfBounds
 * Method to check whether the bird is still on the display screen (the bird can leave if the display window
 * is scaled down quicker than the bird can move.
 *
 * inputs:
 * - xdim: current x dimension of the display window
 * - ydim: current y dimension of the display window
 */
bool Bird::outOfBounds(int xdim, int ydim){
    if(getPosition().x()>xdim || getPosition().y() > ydim){
        return true;
    }
    else{return false;}
}
