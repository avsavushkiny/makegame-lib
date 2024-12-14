#pragma once

#include <Arduino.h>
#include <U8g2lib.h> 
#include "mg.h"

extern Joystick joy;
extern Graphics gfx;

bool stateGame1 = false; //Pong

int8_t xBall{63}, yBall{31}, xx = 2, yy = 1, score1{}, score2{}, posX00, posX11; 


void drawField()
{
    u8g2.drawHLine(0, 0, 128);
    u8g2.drawHLine(0, 63, 128);

    for (int i = 0; i <= 64; i+=6)
    {
        u8g2.drawVLine(64, i, 4);
    }
}

void DrawBall()
{
  u8g2.drawBox(xBall, yBall, 4, 4); xBall += xx; yBall += yy;
  
  if(xBall >= 125) xx = -2;
  if(xBall <= 1) xx = 2;
  if(yBall >= 61) yy = -1;
  if(yBall <= 1) yy = 1;
}

void Calculation()
{
    if (xBall <= 1)
    {
        score2++;
    }
    if (xBall >= 125)
    {
        score1++;
    }

  if ((xBall >= joy.posX0) && (xBall <= joy.posX0 + 5))
  {
    if((yBall >= joy.posY0) && (yBall <= joy.posY0 + 10))
    {
      xx = 2;
    }
  }
    if ((xBall + 3 >= joy.posX1) && (xBall <= joy.posX1))
  {
    if((yBall >= joy.posY1) && (yBall <= joy.posY1 + 10))
    {
      xx = -2;
    }
  }
    if ((score1 == 5) || (score2 == 5))
    {
      stateGame1 = false;

      score1 = 0; score2 = 0;
    }
}

void drawScore()
{
    String score1st{score1}, score2st{score2};
    gfx.print(score1st, 20, 10);
    gfx.print(score2st, 101, 10);
}
void DrawRackets() {
    posX00 = joy.posX0;   posX11 = joy.posX1;

  if (joy.posX0 >= 53) { posX00 = 0; }
  if (joy.posX1 >= 73) { posX11 = 122; }

  joy.updatePositionX0Y0(0, 53, 1, 53);
  u8g2.drawFrame(joy.posX0, joy.posY0, 5, 10);

  joy.updatePositionX1Y1(73, 122, 1, 53);
  u8g2.drawFrame(joy.posX1, joy.posY1, 5, 10);
}


void gamePong()
{
    stateGame1 = true;
    drawField(); DrawRackets(); Calculation(); DrawBall(); drawScore();
}