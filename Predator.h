/* Predator.h
 * Author: Max Elliott 1434717
 * Created On: 2017-12-12
 *
 * Header for Predator objects. Used to represent each predator in the flocking simulation. Essentially a Bird,
 * but with different update method, and the behaviour of chasing other birds and eating them. They have a hunger
 * indicating how many birds they can eat. Predators will be removed once they eat this amount of birds. Inherits
 * from Bird.
 */
#ifndef PREDATOR_H
#define PREDATOR_H
#include <TwoVector.h>
#include <vector>
#include "Bird.h"

class Predator : public Bird{
public:

    //Constructor
    Predator(TwoVector position, double maxSpeed, int heading, int separationDistance, int detectionDistance, int hunger);

    //Desctructor
    virtual ~Predator();

    //new update method for Predators
    void update(std::vector<Bird*>* flock, std::vector<Obstacle*>* obstacles, int xdim, int ydim);

    //new behaviour for predators: chases after the nearest non-predator bird
    TwoVector hunt(std::vector<Bird*>* flock);

    //used to eat Birds
    void eat(Bird* b);

    //inline gettter and setter for new data member
    inline int const getHunger()const{return fHunger;}
    inline int setHunger(int newVal){fHunger= newVal;}

    //checks whether the predator has eaten all the birds it can
    inline bool isFull(){return getHunger()==0;};



private:

    int fHunger;//number of birds the predator can eat

};

#endif // PREDATOR_H
