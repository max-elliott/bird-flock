/* Bird.h
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * Header for Bird objects. Used to represent each bird in the flocking simulation. Each Bird has methods to apply
 * each of its behaviours, which are all called with it's update() method. Inherits from FlockObject.
 */
#ifndef BIRD_H
#define BIRD_H

#include<string>
#include "FlockObject.h"
#include <TwoVector.h>
#include <vector>
#include "Obstacle.h"


class Bird : public FlockObject {
public:

    //Constructor
    Bird(TwoVector position, double maxSpeed, double heading, int separationDistance, int detectionDistance, std::string colour,
         double separationStrength, double cohesionStrength, double alignmentStrength, double avoidPredatorStrength);

    //Destructor
    virtual ~Bird();

    /* Getter methods for data members. Declared inline for faster execution time,
     * at the cost of more memory usage. Declared const so compiler can perform
     * some optimisation */
    inline TwoVector const getVelocity() const{return fVelocity;}
    inline double const getMaxSpeed()const {return fMaxSpeed;}
    inline double const getHeading(){return fHeading;}
    inline int const getSeparationDistance()const {return fSeparationDistance;}
    inline int const getDetectionDistance()const {return fDetectionDistance;}
    inline double const getMaxForce()const{return fMaxForce;}
    inline std::string const getColour()const {return fColour;}
    inline double const getSeperationStrength()const{return fSeparationStrength;}
    inline double const getCohesionstrength()const{return fCohesionStrength;}
    inline double const getAlignmentStrength()const{return fAlignmentStrength;}
    inline double const getAvoidPredatorStrength()const{return fAvoidPredatorStrength;}

    //setters for all variables (except colour and that will never change)
    inline void setVelocity(TwoVector newVal){fVelocity = newVal;}
    inline void setMaxSpeed(double newVal){fMaxSpeed = newVal;}
    inline void setHeading(double newVal){fHeading = newVal;}
    inline void setSeparationDistance(int newVal){fSeparationDistance = newVal;}
    inline void setDetectionDistance(int newVal){fDetectionDistance = newVal;}
    inline void setSeperationStrength(double newVal){fSeparationStrength = newVal;}
    inline void setCohesionstrength(double newVal){fCohesionStrength = newVal;}
    inline void setAlignmentStrength(double newVal){fAlignmentStrength = newVal;}
    inline void setAvoidPredatorStrength(double newVal){fAvoidPredatorStrength = newVal;}

    //method called on all birds to update it's velocity based on its interaction with the rest of the flock
    virtual void update(std::vector<Bird*>* flock,std::vector<Obstacle*>* obstacles, int xdim, int ydim);

    //Behavioural methods that calculate the change in velocity for the bird. These are called in the update method.
    //Each returns a TwoVector 'force' to alter the velocity. Each is due to a different behaviour.
    TwoVector cohesion(std::vector<Bird* >* flock);
    TwoVector separation(std::vector<Bird* >* flock);
    TwoVector alignment(std::vector<Bird* >* flock);
    TwoVector avoidWalls(int xdim, int ydim);
    TwoVector avoidPredators(std::vector<Bird* >* flock);
    TwoVector avoidObstacles(std::vector<Obstacle*>* obstacles);

    //This method is essentially F=ma with m=1. The argument 'TwoVector force' becomes the acceleration, which is added to velocity.
    void applyForce(TwoVector force);

    //Calculates the heading of the Bird, based on the velocity. Used when drawing the Birds.
    void changeHeading();

    //Moves the Bird in the direction of its velocity
    void move();

    //check to make sure the bird is still in the display window dimensions.
    bool outOfBounds(int xdim, int ydim);




private:

    TwoVector fVelocity;//current velocity
    double fMaxSpeed;//max speed allowed
    double fHeading;//angle the Bird is facing towards in degrees
    const double fMaxForce = 0.07;//maximum magnitude a TwoVector from a single behavior method can be
    int fSeparationDistance;//distance Birds want to be apart form each other
    int fDetectionDistance;//Distance Birds can detect other Birds
    std::string fColour;//colour of object, used when drawing objects in DisplayWindow

    //Weightings of each behaviour. avoidWalls and avoidObstacles do not have weighting variable as they cannot be varied; they have a set weighting.
    double fSeparationStrength;
    double fCohesionStrength;
    double fAlignmentStrength;
    double fAvoidPredatorStrength;

};

#endif // BIRD_H
