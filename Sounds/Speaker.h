/*
Author: Nick Witten
Date Last Modified: 4/15/21
Organization: ECE2036 Class
Description:

Implements the Speaker class to
control the game sounds.
*/
#include "mbed.h"
#include "Fire.h"
#include "beats1.h"
#include "beats2.h"
#include "bangsmall.h"
#include "bangmedium.h"
#include "banglarge.h"

#pragma once

/********************************************
Speaker class for controlling game sounds.
********************************************/
class Speaker : public PwmOut
{
public:
    enum ASTERIODS_SOUNDS {NO_SOUND, BEATS1, BEATS2, FIRE, BANG_SMALL, BANG_MEDIUM, BANG_LARGE};
    
    /********************************
    Speaker Constructor
    ********************************/
    Speaker(PinName nPin):PwmOut(nPin)
    {
       m_ulAudioArrayIndex = 0;
       SwitchSound(NO_SOUND);
    }
    
    /********************************
    Plays the next value in the audio
    array.
    ********************************/
    void PlayNextValue()
    {
        if (m_pAudioArray != NULL)
        {
            write((float)m_pAudioArray[ GetNextValue() ]/255.0f);
        }       
    }
    
    /********************************
    Changes the audio array to a new
    sound.
    ********************************/    
    void SwitchSound ( const ASTERIODS_SOUNDS &newSound)
    {
        if (newSound == m_enActiveAudioArray)
        {
            return;
        }
            
        m_ulAudioArrayIndex = 0;
        m_enActiveAudioArray = newSound;
        switch (newSound)
        {           
            case NO_SOUND:
                m_pAudioArray = NULL;
                break;   
            case BEATS1:
                m_pAudioArray = &gBeats1Data[0];
                break;   
            case BEATS2:
                m_pAudioArray = &gBeats2Data[0];
                break;   
            case FIRE:
                 m_pAudioArray = &gFireData[0];
                break;
            case BANG_SMALL:
                m_pAudioArray = &gBangSmallData[0];
                break;
            case BANG_MEDIUM:
                m_pAudioArray = &gBangMediumData[0];
                break;
            case BANG_LARGE:
                m_pAudioArray = &gBangLargeData[0];
                break;              
            default:
                m_pAudioArray = NULL;
                break; 
        }
                             
    }
    
    /***************************************
    Returns the next value in the current
    audio array.
    ***************************************/
    unsigned int GetNextValue()
    {
        m_ulAudioArrayIndex++;
        switch (m_enActiveAudioArray)
        {           
            case BEATS1:
                 m_ulAudioArrayIndex %= NUM_BEATS1_ELEMENTS;
                 if (m_ulAudioArrayIndex == 0)
                 {
                     SwitchSound(BEATS2);
                 }
                 break;
            case BEATS2:
                 m_ulAudioArrayIndex %= NUM_BEATS2_ELEMENTS;
                 if (m_ulAudioArrayIndex == 0)
                 {
                     SwitchSound(BEATS1);
                 }
                 break; 
            case FIRE:
                 m_ulAudioArrayIndex %= NUM_FIRE_ELEMENTS;
                 if (m_ulAudioArrayIndex == 0)
                 {
                     SwitchSound(BEATS1);
                 }
                 break;
            case BANG_SMALL:
                 m_ulAudioArrayIndex %= NUM_BANG_SMALL_ELEMENTS;
                 if (m_ulAudioArrayIndex == 0)
                 {
                     SwitchSound(BEATS1);
                 }                 
                break;
            case BANG_MEDIUM:
                 m_ulAudioArrayIndex %= NUM_BANG_MEDIUM_ELEMENTS;
                 if (m_ulAudioArrayIndex == 0)
                 {
                     SwitchSound(BEATS1);
                 }
                break;
            case BANG_LARGE:
                 m_ulAudioArrayIndex %= NUM_BANG_LARGE_ELEMENTS;
                 if (m_ulAudioArrayIndex == 0)
                 {
                     SwitchSound(BEATS1);
                 }
                 break;              
            default:
                 m_enActiveAudioArray = BEATS1;
                 m_ulAudioArrayIndex %= NUM_BEATS1_ELEMENTS;
                break;          
        }
        return m_ulAudioArrayIndex;
    }
private:
    unsigned long m_ulAudioArrayIndex;
    ASTERIODS_SOUNDS m_enActiveAudioArray;
    const unsigned char *m_pAudioArray;
    
};

            
