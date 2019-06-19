/* TwoVector.h
 * Author: AntoninoSergi, altered by Max Elliott
 * Created on: Sep 25, 2015, altered on 2017-12-02
 *
 * TwoVector is a altered version of Antonio Sergi's ThreeVector class used
 * in the initial exercise. It applies the same concepts, but in 2D rather than 3D.
 * They are used in the simulation for the position and velocity of the FlockObjects,
 * and also as forces to change the velocity of Birds.
 */

#include <math.h>
#include <iostream>

#include "TwoVector.h"

TwoVector::TwoVector(): fX(0),fY(0) {}

TwoVector::TwoVector(double x, double y) : fX(x), fY(y) {}

TwoVector::~TwoVector() {}

TwoVector TwoVector::Unit() const
{
   // return unit vector parallel to this.
   double  tot2 = pow(mag(),2);
   double tot = (tot2 > 0) ?  1.0/sqrt(tot2) : 1.0;
   TwoVector p(fX*tot,fY*tot);
   return p;
}

//Implementation of the global operators; notice the absence of TwoVector::
//They all return a newly constructed TwoVector
TwoVector operator + (const TwoVector & a, const TwoVector & b) {
   return TwoVector(a.x() + b.x(), a.y() + b.y());
}

TwoVector operator - (const TwoVector & a, const TwoVector & b) {
   return TwoVector(a.x() - b.x(), a.y() - b.y());
}

TwoVector operator * (const TwoVector & p, double a) {
   return TwoVector(a*p.x(), a*p.y());
}

TwoVector operator * (double a, const TwoVector & p) {
   return TwoVector(a*p.x(), a*p.y());
}



