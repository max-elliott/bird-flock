/* FlockObject.h
 * Author: Max Elliott
 * Created On: 2017-12-12
 *
 * .cpp file for FlockObject, the very top of the hierarchy of objects to be added to
 * the flock.
 *
 */
#include "FlockObject.h"
#include "TwoVector.h"

/*Constructor
 * Initialises the position and colour, and sets fIsDead to false initially.
 */
FlockObject::FlockObject(TwoVector position) :
    fPosition(position){
    fIsDead = false;
}

//Deconstructor
FlockObject::~FlockObject(){}
