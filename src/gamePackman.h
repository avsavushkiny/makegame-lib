#pragma once

#include <Arduino.h>
#include <U8g2lib.h> 
#include "mg.h"

extern Joystick joy;
extern Graphics gfx;

bool stateGame4 = false;

void drawFieldPackman()
{
u8g2.drawHLine(0, 0 ,128);
u8g2.drawHLine(0, 63 ,128);
u8g2.drawVLine(127, 0, 25);
u8g2.drawVLine(0, 0, 25);
u8g2.drawVLine(127, 39, 25);
u8g2.drawVLine(0, 39, 25);
} 

void gamePackman()
{
    stateGame4 = true;
    drawFieldPackman();
}
