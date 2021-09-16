/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Defines the CommandShip class.
*/
#pragma once
#include <list>
#include "Sprite.h"
#include "Projectile.h"

#define NUM_LASERS 5


/***********************************
CommandShip class to implement the
shape, movement, and firing of 
the controllable ship.
***********************************/
class CommandShip : public Sprite
{
public:    
    CommandShip();
    virtual void move();  
    
    void rotateLeft(); // if rotating right just stop in place
    void rotateRight(); // if rotating right just stop in place
    void fire();
    
    Projectile m_lasers[NUM_LASERS];
    double m_shipLength;

};
