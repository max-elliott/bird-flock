/* Flock.cpp
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * Header file to Flock. Used to store all FlockObjects currently in the simulation.
 * It has runs the simulation by calling the update method and move method of all the Birds. It also
 * removes any dead Birds. It has methods to add and remove each type of FlockObject, and to alter
 * the data members of them.
 *
 */
#ifndef FLOCK_H
#define FLOCK_H

#include <vector>
#include <string>
#include "Bird.h"
#include "Obstacle.h"

class Flock
{
public:

    //Constructor
    Flock();

    //Deconstructor
    virtual ~Flock();

    /* Getter methods for data members. Declared inline for faster execution time,
     * at the cost of more memory usage. Declared const so compiler can perform
     * some optimisation */
    inline std::vector<Bird*>* getBirds(){return fBirds;}
    inline std::vector<Obstacle*>* getObstacles(){return fObstacles;}

    inline const int getBlueCount()const{return fBlueCount;}
    inline const int getGreenCount()const{return fGreenCount;}
    inline const int getPredCount()const{return fPredCount;}
    inline const int getObstacleCount()const{return fObstacleCount;}

    //Setters for data memebers
    inline void setBlueCount(int newVal){fBlueCount =newVal;}
    inline void setGreenCount(int newVal){ fGreenCount=newVal;}
    inline void setPredCount(int newVal){ fPredCount=newVal;}
    inline void setObstacleCount(int newVal){fObstacleCount=newVal;}

    //Method that runs all the actual simulating of the Birds
    void simulateFlock(int xdim, int ydim);

    //add bird to fBirds
    bool addBird(Bird* b);

    //helper method for addBird: checks position isn't blocked by obstacles
    bool checkPositionFree(TwoVector position);

    //remove a FlockObject whose fIsDead==true
    void removeBird(std::string colour, int index);

    //add an obstacle
    void addObstacle(Obstacle* o);

    /* Methods to change the data members of FlockObjects. The data memers of
     * specific colours cn be changed separately to allow different behaviour
     * for different coloured birds. */
    void changeObstacleRadius(int newRadius);
    void changeMaxSpeed(std::string colour, int newSpeed);
    void changeSepDistance(std::string colour, int newSep);
    void changeDetDistance(std::string colour, int newDet);
    void changeHunger(int newHunger);
    void changeSeparationStrength(std::string colour, double newStrength);
    void changeCohesionStrength(std::string colour, double newStrength);
    void changeAlignmentStrength(std::string colour, double newStrength);
    void changeAvoidPredatorStrength(std::string colour, double newStrength);

    //removes all FlockObjects from flock.
    void clearFlock();

private:

    /* Two vectors are used to store all Flock objects, one for all Birds and Predators,
     * and one for all Obstacles. The vectors are declared as pointers so that they can
     * be passed as arguments in methods between classes without having to recreate the
     * vector each time, which would be slow and heavy on memory usage. Each object in
     * the vectors must also be declared as a pointer for the same reasons.
     */
    std::vector<Bird*>* fBirds;
    std::vector<Obstacle*>* fObstacles;

    /* integer counts to keep track of how many of each FlockObject there is. Used by
     * MainWindow to add/remove the right amount of objects when controls are changed.*/
    int fBlueCount;
    int fGreenCount;
    int fPredCount;
    int fObstacleCount;
};

#endif // FLOCK_H
