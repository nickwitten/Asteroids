/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Implements the Projectile functions
to control the shape, color, and movement of 
lasers fired from the command ship.
*/
#include "Projectile.h"
#include "graphic.h"

// Reference points
extern const double maxX;
extern const double maxY;


/************************************
Projectile constructor to set laser
properties.
************************************/
Projectile::Projectile()
{
    m_Length = 5;
    m_Velocity = 160;
    m_color = YELLW;
    m_numPoints = 2;
}
    

/**************************************************
This function resets the laser to a position off
the front of the ship and sets the laser's
direction.
**************************************************/
void Projectile::reset(double dirX, double dirY, double startX, double startY)
{  
    m_xPoints[0] = (startX);
    m_yPoints[0] = (startY); 
    m_xPoints[1] = (startX+dirX*m_Length);
    m_yPoints[1] = (startY+dirY*m_Length); 
    m_DirX = dirX;
    m_DirY = dirY;
    m_bIsValid = true;
}


/*************************************
Overrides sprite move to check for when
lasers go off the screen and sets them
invalid.
*************************************/
void Projectile::move()
{
    // Check if the new position will be off the screen
    if (m_xPoints[1] >= maxX || m_yPoints[1] >= maxY || 
        m_xPoints[1] <= 0 || m_yPoints[1] <= 0)
    {
        m_bIsValid = false;
        draw(true);
    } else {
        Sprite::move();
    }
}