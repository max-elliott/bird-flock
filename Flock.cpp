/* Flock.cpp
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * .cpp file for Flock. Used to store all FlockObjects currently in the simulation.
 * It has runs the simulation by calling the update method and move method of all the Birds. It also
 * removes any dead Birds. It has methods to add and remove each type of FlockObject, and to alter
 * the data members of them.
 *
 */
#include "Flock.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "Bird.h"
#include "Predator.h"
#include "Obstacle.h"
#include <TwoVector.h>

//Constructor: When a Flock is created, it creates a new vector on the heap to store the Birds and Obstacles.
Flock::Flock()
{
    fBirds = new std::vector<Bird*>;
    fObstacles = new std::vector<Obstacle*>;

}

//Deconstructor
Flock::~Flock(){}

/*simulateFlock
 *
 * The method that updates the entire simulation for each frame. It cycles through all Birds (including
 * Predators) and calls their update method. It also checks whether the Bird is dead, and removes it if
 * so. It then cycles through all Obstacles to check whether they're dead, and rmeoves them if so. Once
 * everything is updated, the move method for each Bird is called to update their positions.
 *
 * inputs:
 * - xdim: current x dimension of display window
 * - ydim: current y dimension of display window
 */
void Flock::simulateFlock(int xdim, int ydim){

    //cycle through all birds
    for(int i=0; i < fBirds->size(); i++){

        //if bird is dead, remove it, else call its update method
        Bird* b = fBirds->at(i);
        if(b->getIsDead()){
            removeBird(b->getColour(), i);//remove using custom method, passes index of dead bird
            i--;//so all birds are still checked
        }
        else{
            //update bird, passing fBirds and fFlock pointers to improve runtime performance
            fBirds->at(i)->update(fBirds,fObstacles, xdim, ydim);
        }
    }

    //cycle through all obstacles to remove them if they are dead.
    for(int i=0; i<fObstacles->size(); i++){
        Obstacle* o = fObstacles->at(i);
        if(o->getIsDead()){
            fObstacles->erase(fObstacles->begin()+i);
        }
    }

    //cycle through all birds and move them
    for(int i=0; i<fBirds->size(); i++){
        fBirds->at(i)->move();
    }
}

/* addBird
 *
 * adds a new bird to the flock, if and only if its position is not inside an obstacle.
 *
 * inputs:
 * - b: Bird object to be added
 *
 * return: true if Bird added, false if not
 */
bool Flock::addBird(Bird* b){

    bool checkPos = checkPositionFree(b->getPosition()); //check if position is clear

    //if position is clear, add the bird to the flock, increment appropiate birdCount
    if(checkPos){
        fBirds->push_back(b);

        if(b->getColour().compare("blue")==0){fBlueCount++;}
        else if(b->getColour().compare("green")==0){fGreenCount++;}
        else if(b->getColour().compare("red")==0){fPredCount++;}
    }
    return checkPos;//returns whether Bird was successfully added or not


}

/* Helper method for addBird. Checks whether Bird position is inside an obstacle
 *
 * inputs:
 * - position: position to be checks
 *
 * return: true if position is not in an obstacles
 */
bool Flock::checkPositionFree(TwoVector position){

    for(int i=0; i<getObstacles()->size(); i++){

        Obstacle* o = getObstacles()->at(i);

        //finds distance between position and centre of obstacle (i.e. it's position)
        double distance = (o->getPosition()-  position).mag();

        //if distance is smaller than the obstacle radius, then the position is inside the obstacle
        if(distance <= o->getRadius()){
            return false;
        }
    }
    return true;
}

/* removeBird
 * Method used to remove any bird that has fIsDead = true.
 *
 * input:
 * - colour: colour of bird to be removed
 * - index: position in the vector fBirds of the Bird to be removed
 */
void Flock::removeBird(std::string colour, int index){
    fBirds->erase(fBirds->begin() + index);//delete Bird

    //decrement appropriate birdCount
    if(colour.compare("blue")==0){fBlueCount--;}
    else if(colour.compare("green")==0){fGreenCount--;}
    else if(colour.compare("red")==0){fPredCount--;}
}

//adds obstacles to fObstacle
void Flock::addObstacle(Obstacle* o){
    fObstacles->push_back(o);
}

//changes radius of all obstacles
void Flock::changeObstacleRadius(int newRadius){
    for(int i=0; i<fObstacles->size(); i++){
        fObstacles->at(i)->setRadius(newRadius);
    }
}

//changes maxSpeed of a specific colour of Bird (blue, green or red)
void Flock::changeMaxSpeed(std::string colour, int newSpeed){
    for(int i=0; i<fBirds->size(); i++){
        if(fBirds->at(i)->getColour().compare(colour) == 0){
            fBirds->at(i)->setMaxSpeed(newSpeed);
        }
    }
}

//changes separation distance of a specific colour of Bird (blue, green or red)
void Flock::changeSepDistance(std::string colour, int newSep){
    for(int i=0; i<fBirds->size(); i++){
        if(fBirds->at(i)->getColour().compare(colour) == 0){
            fBirds->at(i)->setSeparationDistance(newSep);
        }
    }
}

//changes detection distance of a specific colour of Bird (blue, green or red)
void Flock::changeDetDistance(std::string colour, int newDet){
    for(int i=0; i<fBirds->size(); i++){
        if(fBirds->at(i)->getColour().compare(colour) == 0){
            fBirds->at(i)->setDetectionDistance(newDet);
        }
    }
}

//changes hunger of Predators
void Flock::changeHunger(int newHunger){
    for(int i=0; i<fBirds->size(); i++){
        if(fBirds->at(i)->getColour().compare("red")==0){
            Predator* p = dynamic_cast<Predator*>(fBirds->at(i));//need to specifically cast to Predator to call Predator-specific method.
            p->setHunger(newHunger);
        }
    }
}

//changes separation force weighting of a specific colour of Bird (blue, green or red)
void Flock::changeSeparationStrength(std::string colour, double newStrength){
    for(int i=0; i<fBirds->size(); i++){
        if(fBirds->at(i)->getColour().compare(colour) == 0){
            fBirds->at(i)->setSeperationStrength(newStrength);
        }
    }
}

//changes cohesion force weighting of a specific colour of Bird (blue or green)
void Flock::changeCohesionStrength(std::string colour, double newStrength){
    for(int i=0; i<fBirds->size(); i++){
        if(fBirds->at(i)->getColour().compare(colour) == 0){
            fBirds->at(i)->setCohesionstrength(newStrength);
        }
    }
}

//changes alignment force weighting of a specific colour of Bird (blue or green)
void Flock::changeAlignmentStrength(std::string colour, double newStrength){
    for(int i=0; i<fBirds->size(); i++){
        if(fBirds->at(i)->getColour().compare(colour) == 0){
            fBirds->at(i)->setAlignmentStrength(newStrength);
        }
    }
}

//changes avoidPredator force weighting of a specific colour of Bird (blue, green or red)
void Flock::changeAvoidPredatorStrength(std::string colour, double newStrength){
    for(int i=0; i<fBirds->size(); i++){
        if(fBirds->at(i)->getColour().compare(colour) == 0){
            fBirds->at(i)->setAvoidPredatorStrength(newStrength);
        }
    }
}

//removes all FlockObjects from the vectors
void Flock::clearFlock(){
    fBirds->clear();
    fObstacles->clear();
}


