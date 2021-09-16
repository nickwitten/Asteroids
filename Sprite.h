/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Defines the Sprite class.
*/
#pragma once

#include <vector>
#define NUM_POINTS 5 // reduce if you need to

using namespace std;

// Objects are created centered around the origin

/************************************************
Base class for all drawings on the LCD.
Implements common functionality for moving,
drawing, and setting properties for objects
on the screen.
************************************************/
class Sprite
{
public:
    enum SPRITE_TYPES {SHIP, FIRE, SMALL_AST, MEDIUM_AST, LARGE_AST};
    Sprite();
    virtual ~Sprite(){};
    
    virtual void move();
    virtual void draw(bool bErase);
    virtual void addPoint(int x, int y);
    void setColor(int color){m_color = color;}
    void setNumPoints(int num){m_numPoints = num;} 
    void setDirection(double u, double v);
    void setInvalid(){m_bIsValid = false;}
    vector<vector<vector<double> > > getLineSegments();
    virtual bool isValid() { return m_bIsValid;}  
      
protected:
    double m_xCenter, m_yCenter;  // center of mass
    double m_DirX, m_DirY; // unit vector
    float m_Velocity;
    
    int m_color;
    
    double m_RotationAmount;
    double m_RotationVelocity; // degrees/sec
    int m_RotationDirection; // -1 is left, 0 is not rotating , 1 is rotating right   
    
    int m_numPoints;
    
    bool m_bIsValid; 
   
    double m_xPoints[NUM_POINTS];
    double m_yPoints[NUM_POINTS];    
};


