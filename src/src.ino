#include "mg.h"
Graphics gfx;
Joystick Joy;
void drawField() {
  u8g2.drawHLine(0, 0, 128);
  u8g2.drawHLine(0, 63, 128);

  for (int a = 0; a <= 64; a = a + 6) 
  {
    u8g2.drawVLine(64, a, 4);
  }
}
void drawRackets()
{
  Joy.updatePositionXY();

u8g2.drawFrame(5,Joy.posY0,5,10);
u8g2.drawFrame(117,Joy.posY1,5,10);
}
int xBall{63}; int yBall{31};
void drawBall()
{
  int xx{2}; int yy{1}; 
  u8g2.drawBox(xBall,yBall,4,4);
  xBall += xx ;
  yBall += yy ;
if(xBall >= 128) xx = -2;
if(xBall <= 0) xx =2;
if(yBall>= 64) xx = -1;
if(yBall <= 0) xx =1;




}
void gamePong()
{
  drawField();
  drawRackets();
  drawBall();
}
void setup() {
  gfx.initializationSystem();
}

void loop() {
  gfx.render(gamePong, 10);
}