/* FlockObject.h
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * Header file for FlockObject, the very top of the hierarchy of objects to be added to
 * the flock. All objects that will be added to the flock will inherit from the class,
 * either directly or indirectly. This guarantees all objects have a position and colour
 * so they can be drawn by the simulation. It also guarantees they have an fIsDead data
 * member, so they can be removed from the simulation when needed.
 *
 */
#ifndef FLOCKOBJECT_H
#define FLOCKOBJECT_H
#include "TwoVector.h"
#include <string>

class FlockObject
{
public:

    //Constructor
    FlockObject(TwoVector fPosition);

    //Deconstructor
    virtual ~FlockObject();

    /* Getter methods for data members. Declared inline for faster execution time,
     * at the cost of more memory usage. Declared const so compiler can perform
     * some optimisation */
    inline double const getXPos()const {return fPosition.x();}
    inline double const getYPos()const{return fPosition.y();}
    inline TwoVector const getPosition() const{return fPosition;}
    inline bool const getIsDead()const{return fIsDead;}

    //Setters for data members
    inline void setXPos(double newVal){fPosition.SetX(newVal);}
    inline void setYPos(double newVal){fPosition.SetY(newVal);}
    inline void setIsDead(bool newVal){fIsDead = newVal;}

private:

    TwoVector fPosition;//current position of object, from top-left of display screen
    bool fIsDead; //indicates whther the object needs to be removed by the simulation or not

};

#endif // FLOCKOBJECT_H
