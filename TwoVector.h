/* TwoVector.h
 * Author: Antonino Sergi, altered by Max Elliott
 * Created on: Sep 25, 2015, altered on 2017-12-02
 *
 * TwoVector is a altered version of Antonio Sergi's ThreeVector class used
 * in the initial exercise. It applies the same concepts, but in 2D rather than 3D.
 * They are used in the simulation for the position and velocity of the FlockObjects,
 * and also as forces to change the velocity of Birds.
 */

#ifndef TWOVECTOR_H_
#define TWOVECTOR_H_

#include<cmath>

class TwoVector {
public:

        //Constructors.
        TwoVector();
        TwoVector(double x, double y);


        //Destructor.
        virtual ~TwoVector();

        //Declaration of access methods. inline instructs the compiler to replicate
        //the corresponding machine code each time they are invoked, rather than
        //jumping to the position of a single copy of machine code; this results in
        //faster execution, but larger executables. Declaring a method inline requires
        //the implementation to be done in the same file. const indicates that the object is not
        //modified by the execution of the method, thus the compiler can perform some
        //optimization
        //The components in cartesian coordinate system.
        inline double x()  const;
        inline double y()  const;

        //inline but not const, since they are meant to modify the object
        //Set the components
        inline void SetX(double);
        inline void SetY(double);

        //Returns magnitude of a TwoVector
        inline double mag()const;

        //Declaration of operators acting on the invoking instance; they could be implemented
        //here or in the implementation file, if they were not declared inline
        //Assignment.
        inline TwoVector & operator = (const TwoVector &);

        //Addition.
        inline TwoVector & operator += (const TwoVector &);

        //Subtraction.
        inline TwoVector & operator -= (const TwoVector &);

        //Unary minus.
        inline TwoVector operator - () const;

        //Unit vector parallel to this.
        TwoVector Unit() const;

private:

        double fX, fY;//x and y compenent of each vector

};

//Declaration of operators without an invoking instance. They must be global, thus
//declared outside the scope of the class
//Addition of 2-vectors.
TwoVector operator + (const TwoVector &, const TwoVector &);

//Subtraction of 2-vectors.
TwoVector operator - (const TwoVector &, const TwoVector &);

//Scaling of 2-vectors with a real number
TwoVector operator * (const TwoVector &, double a);
TwoVector operator * (double a, const TwoVector &);

//Implementation of all methods and operators declared inline
inline double TwoVector::x()  const { return fX; }
inline double TwoVector::y()  const { return fY; }

inline void TwoVector::SetX(double newVal) { fX = newVal; }
inline void TwoVector::SetY(double newVal) { fY = newVal; }

inline double TwoVector::mag()const{
    return pow(fX*fX + fY*fY, 0.5);
}

//All operators involving assignment return the invoking instance itself
//by dereferencing the pointer this
inline TwoVector & TwoVector::operator = (const TwoVector & p) {
   fX = p.fX;
   fY = p.fY;
   return *this;
}

inline TwoVector& TwoVector::operator += (const TwoVector & p) {
   fX += p.fX;
   fY += p.fY;
   return *this;
}

inline TwoVector& TwoVector::operator -= (const TwoVector & p) {
   fX -= p.fX;
   fY -= p.fY;
   return *this;
}

inline TwoVector TwoVector::operator - () const {
   return TwoVector(-fX, -fY);
}


#endif /* TWOVECTOR_H_ */
