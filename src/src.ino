#include "mg.h"

Joystick joy;
Graphics gtx;

void readDataJoy() 
{
joy.updatePositionXY();

  if (joy.pressKeyA() == true)
  {
    gtx.print(13, (String)joy.posX1 + "l" + (String)joy.posY1, 0, 13, 13, 8);
  } 
  else
  {
    gtx.print(13, "no joy", 0, 13, 13, 8);
    u8g2.drawFrame(5, joy.posY1, 5, 15);
  } 
}

void setup() {
  gtx.initializationSystem();
}

void loop() {
  gtx.render(readDataJoy);
}