#ifndef Collision
#define Collision

#include "2dDrawn.hpp"
#include<stdio.h>
#include <vector>
#include <typeinfo>

inline bool collision(D2 A, D2 B){
     //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A


    leftA = A.location.first;
    rightA = A.location.first + A.size.first;
    topA = A.location.second;
    bottomA = A.location.second + A.size.second;

    //Calculate the sides of rect B
    leftB = B.location.first;
    rightB = B.location.first + B.size.first;
    topB = B.location.second;
    bottomB = B.location.second + B.size.second;
 if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}




#endif