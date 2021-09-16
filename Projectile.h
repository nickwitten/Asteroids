/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Defines the Projectile class.
*/
#pragma once

#include "Sprite.h"

/**********************************
Projectile class to implement the
shape, color, and movement of 
lasers fired from the command ship.
**********************************/
class Projectile : public Sprite
{
public:
    Projectile();
    virtual void move();
    void reset(double dirX, double dirY, double startX, double startY);
protected:
    int m_Length;
};