/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Driver code to set up the mbed hardware 
and then begin the game loop.  This
also contains interrupts to handle
user inputs.
*/
#include "mbed.h"
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include "Speaker.h"
#include "PinDetect.h"
#include "uLCD_4DGL.h"
#include "CommandShip.h"
#include "Asteroid.h"
#include "graphic.h"


using namespace std;
#define GAME_PAUSED 0
#define GAME_RUNNING 1
#define GAME_OVER 2

#define NUM_ASTEROIDS 12

////////////////////////////////////////
// Setup instance of LCD display
uLCD_4DGL guLCD(p28, p27, p29); // serial tx, serial rx, reset pin;
////////////////////////////////////////
// Setup instances of push button pins
PinDetect gPB_left(p16); 
PinDetect gPB_right(p17); 
PinDetect gPB_fire(p18);

// Variable indicates if game is paused or running
int gGameState = GAME_PAUSED;
// Declare and initialize the speaker
Speaker gSpeakerOut(p21);

// declare the gTimeStep
double gTimeStep = 0.03;
int gNumLives = 3;

// LCD Reference Points
double gOriginX = 63.0;
double gOriginY = 63.0;
double maxX = 127;
double maxY = 127;

// Create Ship and Asteriods
CommandShip gShip;
Asteroid gAsteroids[NUM_ASTEROIDS];


//////////////////////////////////////////////////////////////////////
// Interrupt routine
// used to output next analog sample whenever a timer interrupt occurs
void Sample_timer_interrupt(void)
{
    // Call speaker function to play next value
    gSpeakerOut.PlayNextValue();
}
//---------------------------------------------------------------------------------------------------
// Callback routine is interrupt activated by a debounced pb_left hit
void pb_left_hit_callback (void)
{
    // Update game state and tell ship to rotate to the left
    if (gGameState == GAME_RUNNING)
    {
        gShip.rotateLeft();
    }
    
    gGameState = GAME_RUNNING;
 
}
//---------------------------------------------------------------------------------------------------
// Callback routine is interrupt activated by a debounced pb_right hit
void pb_right_hit_callback (void)
{
    // Update game state and tell ship to rotate to the left
    if (gGameState == GAME_RUNNING)
    {
 
        gShip.rotateRight();
    }
    
    gGameState = GAME_RUNNING;

}
//---------------------------------------------------------------------------------------------------
// Callback routine is interrupt activated by a debounced pb_fire hit
void pb_fire_hit_callback (void)
{
    // Update game state and tell ship to fire
    if (gGameState == GAME_RUNNING)
    {
        gShip.fire();
    }
    gGameState = GAME_RUNNING;
}

//---------------------------------------------------------------------------------------------------

int main()
{
    
    srand(static_cast<unsigned int>(time(0)));

    // Setup push buttons
    gPB_left.mode(PullUp);
    gPB_right.mode(PullUp);
    gPB_fire.mode(PullUp);
    // Delay for initial pullup to take effect
    wait(.01);
    // Setup Interrupt callback functions for a pb hit
    gPB_left.attach_deasserted(&pb_left_hit_callback);
    gPB_right.attach_deasserted(&pb_right_hit_callback);
    gPB_fire.attach_deasserted(&pb_fire_hit_callback);

    // Setup speaker
    gSpeakerOut.period(1.0/100000.0);  
    // set up a timer to be used for sample rate interrupts
    Ticker Sample_Period;      
    Sample_Period.attach(&Sample_timer_interrupt, 1.0/(20000.0));

    //Setup LCD display
    guLCD.display_control(LANDSCAPE);
    guLCD.background_color(BLACK);
    guLCD.cls();
    guLCD.baudrate(BAUD_3000000); //jack up baud rate to max for fast display
    wait(1.0);
  
    // Start sampling pb inputs using interrupts
    gPB_left.setSampleFrequency();
    gPB_right.setSampleFrequency();
    gPB_fire.setSampleFrequency();

    //////////////////////////////////////
    // Everything should be ready to start playing the game.
    while(1)
    {
        // Reset lives
        gNumLives = 3;
        
        // reset asteroids
        for (int i = 0; i < NUM_ASTEROIDS; ++i)
        {
            gAsteroids[i].setInvalid();
        }
        // reset lasers
        for (int i = 0; i < NUM_LASERS; ++i)
        {
            gShip.m_lasers[i].setInvalid();
        }
        
        // Which asteroid index will be next created
        int astCtr = 0;
        
        guLCD.cls();
        // Ask the user if they would like to play a game. 
        guLCD.printf("Would you like to play a game?\n\n Press Any Key to Start");
        
        wait(.01);
        // Wait for a button to be pressed
        gGameState = GAME_PAUSED;
              
        while (gGameState == GAME_PAUSED)
        {

            wait(0.1);
        }

        guLCD.cls();
        
        // Start up new game
        gSpeakerOut.SwitchSound(Speaker::BEATS1);
        // Create Initial Asteriods
        const int maxAstSize = 15;
        const int AstBuffer = 2;
        gAsteroids[astCtr++] = Asteroid(LARGE,
            0 + maxAstSize + AstBuffer, 0 + maxAstSize + AstBuffer);
        gAsteroids[astCtr++] = Asteroid(LARGE,
            maxX - (maxAstSize + AstBuffer), 0 + maxAstSize + AstBuffer);
        gAsteroids[astCtr++] = Asteroid(LARGE,
            maxX - (maxAstSize + AstBuffer), maxY - (maxAstSize + AstBuffer));
        gAsteroids[astCtr++] = Asteroid(LARGE,
            0 + maxAstSize + AstBuffer, maxY - (maxAstSize + AstBuffer));
            
        //gAsteroids[3] = Asteroid(0, 63, 90);
        //gAsteroids[3].setDirection(0, -1);
        
        // Start Game loop

        while (gNumLives > 0)
        {
            
            clock_t start = clock();  // start time
                        
            
            // Move the ship and the asteriods
            gShip.move();
            
            // Flag to check if there are still valid asteroids
            bool noAsteroids = true;
            
            for (int ii = 0 ; ii < NUM_ASTEROIDS ; ++ii)
            {
                if (gAsteroids[ii].isValid())
                {
                    // Set flag to false
                    noAsteroids = false;
                    
                    gAsteroids[ii].move();
                    
                    // Check if it hit the ship
                    if (gAsteroids[ii].checkCollision(gShip))
                    {
                        wait(1);
                        // Decrement lives
                        --gNumLives;
                        // If Game Was Lost Print
                        if (gNumLives == 0)
                        {
                            guLCD.printf("You Lost");
                            wait(2);
                        }
                        // Tell them how many lives are left
                        else
                        {
                            guLCD.cls();
                            guLCD.printf("%i Lives Left", gNumLives);
                            wait(2);
                            guLCD.cls();
                        }
                        // Break the asteroid that hit the ship
                        gAsteroids[ii].setInvalid();
                        gAsteroids[ii].draw(true);
                    }
                    
                    // Check if any lasers hit the asteroid
                    for (int jj = 0; jj < NUM_LASERS; ++jj)
                    {
                        if (gShip.m_lasers[jj].isValid() && 
                            gAsteroids[ii].checkCollision(gShip.m_lasers[jj]))
                        {
                            // Break the asteroid
                            vector<Asteroid> children = gAsteroids[ii].breakAsteroid();
                            if (children.size() > 0)
                            {
                                // Set asteroid to first child
                                gAsteroids[ii] = children[0];
                                // Create a new asteroid spot for second child
                                gAsteroids[astCtr++] = children[1];
                            }
                            // Destroy the laser
                            gShip.m_lasers[jj].setInvalid();  
                            gShip.m_lasers[jj].draw(true);
                        }
                    }
                }
            }
            
            // Check if all asteriods are invalid and exit game if that is the case
            if (noAsteroids)
            {
                guLCD.cls();
                guLCD.printf("You Won!");
                gSpeakerOut.SwitchSound(Speaker::NO_SOUND);
                wait(5);
                break;
            }
            
            // End time
            clock_t end = clock();
            
            // If loop was quicker than gTimeStep wait that amount
            long double time_left = 100 * gTimeStep - (end - start);
            if (time_left > 0)
            {
                wait(time_left);
            }
            
        }

        gGameState = GAME_PAUSED;
    }

} //end main

