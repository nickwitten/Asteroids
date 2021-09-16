/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Implements the functions in the CommandShip 
class to control the shape, 
movement, and firing of 
the controllable ship.
*/
#include <iterator>
#include "./Sounds/Speaker.h"
#include "CommandShip.h"
#include "graphic.h"
#include "Projectile.h"


const double DEFAULT_ROTATION_SPEED = 12.0;

extern Speaker gSpeakerOut;

// Reference points
extern double gOriginX;
extern double gOriginY;

/*************************************
CommandShip constructor to initialize
member variables.
*************************************/
CommandShip::CommandShip()
{
    m_shipLength = 16;

    // Top of CommandShip
    m_xPoints[0] = gOriginX;
    m_yPoints[0] = (m_shipLength/2 + gOriginY);
    //Bottom left of CommandShip
    m_xPoints[1] = (m_shipLength/4 + gOriginX);
    m_yPoints[1] = (-m_shipLength/2 + gOriginY);
    //Bottom left of CommandShip
    m_xPoints[2] = (-m_shipLength/4 + gOriginX);
    m_yPoints[2] = (-m_shipLength/2 + gOriginY);    
    
    // Set color to red
    m_color = _RED_;
    
    m_numPoints = 3;
    
    m_Velocity = 0.0f;  // Command ship has no linear velocity
    m_RotationVelocity = 0.0f; // degrees/sec
    m_RotationDirection = NONE;  
}

/***************************************************
Fire function calls reset on one of the ship's
lasers and sets its direction based on the 
orientation of the ship and plays a sound.
***************************************************/
void CommandShip::fire()
{
    // Enable a projectile
    for (int ii = 0 ; ii < NUM_LASERS ; ++ii)
    {
        if (!m_lasers[ii].isValid())
        {
            // Get direction of ship
            double dirX = (m_xPoints[0] - gOriginX) / (m_shipLength / 2);
            double dirY = (m_yPoints[0] - gOriginY) / (m_shipLength / 2);
            double startX = m_xPoints[0];
            double startY = m_yPoints[0];
            m_lasers[ii].reset(dirX, dirY, startX, startY);
            break;
        }
    }
 
     gSpeakerOut.SwitchSound(Speaker::FIRE);
}

/******************************************
Overrides base class move function to
call move on the lasers as well.
******************************************/
void CommandShip::move()
{
    
    Sprite::move();
         
    for (int ii = 0 ; ii < NUM_LASERS ; ii++)
    {
        if (m_lasers[ii].isValid())
        {
            m_lasers[ii].move();
        }
    }
}

/****************************************
Changes direction to rotate left if not
rotating and stopped if rotating right.
****************************************/
void CommandShip::rotateLeft()
{
    
     // if rotating right just stop in place
    if (m_RotationDirection == RIGHT)
    {
        m_RotationDirection = NONE;
        m_RotationVelocity = 0.0;
    }
    else
    {
        m_RotationDirection = LEFT;
        m_RotationVelocity = DEFAULT_ROTATION_SPEED;
    }    
}

/******************************************
Changes direction to rotate right if not
rotating and stopped if rotating left.
******************************************/
void CommandShip::rotateRight()
{
     // if rotating right just stop in place
    if (m_RotationDirection ==  LEFT)
    {
        m_RotationDirection = NONE;
        m_RotationVelocity = 0.0;
    }
    else
    {
        m_RotationDirection = RIGHT;
        m_RotationVelocity = -DEFAULT_ROTATION_SPEED;
    }    
}
