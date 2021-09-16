/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Implements the sprite functions to control
the moving,drawing, and setting properties for
objects on the screen.
*/
#include <math.h>
#include "Sprite.h"
#include "uLCD_4DGL.h"
#include "graphic.h"
#include <vector>


extern uLCD_4DGL guLCD;
extern double gTimeStep;
extern double gOriginX;
extern double gOriginY;

/*******************************
Sprite constructor initializes
member variables.
*******************************/
Sprite::Sprite():
m_xCenter(gOriginX),
m_yCenter(gOriginY),
m_DirX(0.0), 
m_DirY(0.0),
m_Velocity(0.0),
m_color(WHITE),
m_RotationVelocity(0.0),
m_RotationDirection(NONE),
m_numPoints(0), 
m_bIsValid(false)
{
 
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// move   
// Erase Sprite from current location
// Move the sprite to the new location 
// Draw sprite at new location
void Sprite::move()
{

    // Erase object
    draw(true);
 
    // Translate to the origin all points around the origin using the center points   
    for (int ii = 0 ; ii < m_numPoints ; ++ii)
    {
        m_xPoints[ii] -= m_xCenter;
        m_yPoints[ii] -= m_yCenter;
    }     
 
    // Rotate around the origin
    float tmpX, tmpY;
    for (int ii = 0 ; ii < m_numPoints ; ++ii)
    {
        tmpX = m_xPoints[ii]*std::cos(m_RotationVelocity*PI/180.0) - m_yPoints[ii]*std::sin(m_RotationVelocity*PI/180.0);
        tmpY = m_yPoints[ii]*std::cos(m_RotationVelocity*PI/180.0) + m_xPoints[ii]*std::sin(m_RotationVelocity*PI/180.0); 
        
        m_xPoints[ii] = tmpX;
        m_yPoints[ii] = tmpY;     
    }    
    // Translate back to original location
    for (int ii = 0 ; ii < m_numPoints ; ++ii)
    {
        m_xPoints[ii] += m_xCenter;
        m_yPoints[ii] += m_yCenter;
    }          
    // Move to new location
    for (int ii = 0 ; ii < m_numPoints ; ++ii)
    {
        m_xPoints[ii] += m_DirX*m_Velocity*(gTimeStep);
        m_yPoints[ii] += m_DirY*m_Velocity*(gTimeStep);
    } 
    m_xCenter += m_DirX*m_Velocity*(gTimeStep);
    m_yCenter += m_DirY*m_Velocity*(gTimeStep);
    // Draw at the new location    
    draw(false);
 
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// draw sprite at new location
void Sprite::draw(bool bErase)
{
    int currentColor = BLACK;
    if (!bErase)
    {
        currentColor = m_color;
    }
    
    if (m_numPoints < 2)
    {
        return;
    }
    else if (m_numPoints == 2)
    {
        guLCD.line((int)m_xPoints[m_numPoints-1], (int)m_yPoints[m_numPoints-1] , (int)m_xPoints[0], (int)m_yPoints[0], currentColor);
    }
    else
    {       
        for (int ii = 0 ; ii < (m_numPoints-1) ; ++ii)
        {
            guLCD.line((int)m_xPoints[ii], (int)m_yPoints[ii] , (int)m_xPoints[ii+1], (int)m_yPoints[ii+1], currentColor);
        }
        // go from last point to first point
        guLCD.line((int)m_xPoints[m_numPoints-1], (int)m_yPoints[m_numPoints-1] , (int)m_xPoints[0], (int)m_yPoints[0], currentColor);
    }

}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// addPoint
// Adds a point to the vector of points
void Sprite::addPoint(int x, int y)
{
    m_xPoints[m_numPoints] = x;
    m_yPoints[m_numPoints] = y;
    m_numPoints++;
    if (m_numPoints == NUM_POINTS)
    {
        m_numPoints = 0;
    }
}

/********************************************
Sets the direction of the object to unit
vector (u, v)
********************************************/
void Sprite::setDirection(double u, double v)
{
    m_DirX = u;
    m_DirY = v;
}

/*******************************************
Returns the line segments of the object in
a multidimensional vector of the form:
[ [ [PointX1, PointY1], [PointX2, PointY2] ],
  [ [PointX1, PointY1], [PointX2, PointY2] ] ]
*******************************************/
vector<vector<vector<double> > > Sprite::getLineSegments()
{
    // Initialize vector
    vector<vector<vector<double> > > segments;
    // Loop through points
    for (int ii = 0; ii < m_numPoints - 1; ++ii)
    {
        // Initialize segment vector
        vector<vector<double> > segment;
        // Create Point1
        vector<double> point1;
        point1.push_back(m_xPoints[ii]);
        point1.push_back(m_yPoints[ii]);
        segment.push_back(point1);
        // Create Point2
        int nextPoint = ii + 1;
        if (nextPoint == m_numPoints)
        {
            nextPoint = 0;
        }
        vector<double> point2;
        point2.push_back(m_xPoints[nextPoint]);
        point2.push_back(m_yPoints[nextPoint]);
        segment.push_back(point2);
        segments.push_back(segment); 
    }
    return segments;
}
