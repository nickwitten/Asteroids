/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Implements Asteroid functions to control the shape,
movement, collision detection, and
destruction of asteroids.
*/

#include <iterator>
#include <cmath>
#include <ctime>
#include "mbed.h"
#include "graphic.h"
#include "Asteroid.h"
#include "./Sounds/Speaker.h"


// Reference points
extern double gOriginX;
extern double gOriginY;
extern double maxX;
extern double maxY;
// Speaker
extern Speaker gSpeakerOut;


/********************************************
Asteroid constructor to set size, position,
rotational velocity, random shape, and random
rotational direction.
********************************************/
Asteroid::Asteroid(int size, double x, double y)
{   

    // Set size and position
    m_size = size;
    m_xCenter = x;
    m_yCenter = y;
    m_numPoints = 5;
    
    // Set random direction
    // Double between -1 and 1 by 0.01
    double xdirection = 1 - rand() % 201 / 100;
    // Solve for y to make unit vector
    double ydirection = sqrt(1.0 - pow(xdirection, 2.0));
    // Random up or down direction
    int yDown = rand() % 2;
    if (yDown)
    {
        ydirection = -ydirection;
    }
    m_DirX = xdirection;
    m_DirY = ydirection;
    
    // Get values based on size
    double maxRad;
    double velocity;
    double rVelocity;
    switch (size)
    {
        case SMALL:
            maxRad = 5.0;
            velocity = 40.0;
            rVelocity = 30.0;
            break;
        case MEDIUM:
            maxRad = 10.0;
            velocity = 24.0;
            rVelocity = 20.0;
            break;
        case LARGE:
            maxRad = 15.0;
            velocity = 16.0;
            rVelocity = 10.0;
            break;
        default:
            break;
    }
    // Generate random points between half and max radius
    double minRad = maxRad / 2.0;
    for (int ii = 0; ii < m_numPoints; ++ii)
    {
        double range = maxRad - minRad;
        double rad = (rand() % static_cast<int>(range * 2 + 1)) / 2.0 + minRad;
        double angle = (2.0 * PI) / m_numPoints * ii;
        m_xPoints[ii] = m_xCenter + rad * std::cos(angle);
        m_yPoints[ii] = m_yCenter + rad * std::sin(angle);
    }
    // Set velocities
    m_Velocity = velocity;
    m_RotationVelocity = rVelocity;
    // Random rotation direction
    if (rand() % 2)
    {
        m_RotationDirection = 1;
    }
    else
    {
        m_RotationDirection = -1;
    }
    // Make valid
    m_bIsValid = true;
}

/**********************************
Adds bounds checkings so the asteroid
bounces off the edge of the 
screen.
**********************************/
void Asteroid::move()
{
    Sprite::move();
    if (m_xCenter <= 0)
    {
        setDirection(abs(m_DirX), m_DirY);
    }
    if (m_yCenter <= 0)
    {
        setDirection(m_DirX, abs(m_DirY));
    }
    if (m_xCenter >= maxX)
    {
        setDirection(-abs(m_DirX), m_DirY);
    }
    if (m_yCenter >= maxY)
    {
        setDirection(m_DirX, -abs(m_DirY));
    }
}

/******************************************
Checks whether the sprite passed in has
any overlapping segments with the asteroid.
Returns true or false.
******************************************/
bool Asteroid::checkCollision(Sprite &InSprite)
{
    // Get line segments in both objects
    vector<vector<vector<double> > > astSegments = getLineSegments();
    vector<vector<vector<double> > > inSegments = InSprite.getLineSegments();
    for (int ii = 0; ii < astSegments.size(); ++ii)
    {
        for (int jj = 0; jj < inSegments.size(); ++jj)
        {
            // For each line segment check if they overlap
            vector<double> A = astSegments[ii][0];
            vector<double> B = astSegments[ii][1];
            vector<double> C = inSegments[jj][0];
            vector<double> D = inSegments[jj][1];
                    
            if (ccw(A, C, D) != ccw(B, C, D) and ccw(A, B, C) != ccw(A, B, D))
            {
                // They overlap
                return true;
            }
        }
    }
    // None overlapped
    return false;
}

/**********************************************
This function returns a vector of children of the
broken asteroid.  If the asteroid is large the
children will contain a medium and a small
asteroid.  If the asteroid is medium the
children will contain two small asteroids.
If the asteroid is small the children will contain
no asteroids.
**********************************************/
vector<Asteroid> Asteroid::breakAsteroid()
{
    vector<Asteroid> children;
    if (m_size == LARGE)
    {
        Asteroid child1(MEDIUM, m_xCenter, m_yCenter);
        child1.setDirection(-m_DirX, -m_DirY);
        Asteroid child2(SMALL, m_xCenter, m_yCenter);
        child2.setDirection(-m_DirY, m_DirX);
        children.push_back(child1);
        children.push_back(child2); 
        gSpeakerOut.SwitchSound(Speaker::BANG_LARGE);   
    }
    else if (m_size == MEDIUM)
    {
        Asteroid child1(SMALL, m_xCenter, m_yCenter);
        child1.setDirection(-m_DirY, m_DirX);
        Asteroid child2(SMALL, m_xCenter, m_yCenter);
        child2.setDirection(m_DirY, -m_DirX);
        children.push_back(child1);
        children.push_back(child2);
        gSpeakerOut.SwitchSound(Speaker::BANG_MEDIUM);    
    }
    else
    {
        gSpeakerOut.SwitchSound(Speaker::BANG_SMALL);    
    }
    m_bIsValid = false;  // Destroy self
    draw(true);  // Erase self
    return children;
}

/*****************************************
Helper function to check whether three 
points make a counterclockwise rotation
*****************************************/
bool ccw(vector<double> A, vector<double> B, vector<double> C)
{
    return (A[1] - C[1]) * (B[0] - A[0]) > (A[1] - B[1]) * (C[0] - A[0]);
}
