/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Defines the Asteroid class.
*/

#pragma once

#include <vector>
#include "Sprite.h"


/******************************************
Asteroid class to implement the shape,
movement, collision detection, and
destruction of asteroids.
******************************************/
class Asteroid : public Sprite
{
public:
    Asteroid(){};
    Asteroid(int size, double x, double y);
    virtual void move();
    bool checkCollision(Sprite &InSprite); 
    vector<Asteroid> breakAsteroid();
private:
    int m_size;
};

/*****************************************
Helper function to check whether three 
points make a counterclockwise rotation
*****************************************/
bool ccw(vector<double> A, vector<double> B, vector<double> C);