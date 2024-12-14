#include "mg.h"
#include "xbmp.h"
#include "gamePong.h"
#include "gameAliens.h"
#include "gameSchoolboy.h"
#include "gamePackman.h"

Joystick joy;
Graphics gfx;
Cursor crs, crs2;
Shortcut iconSapper, iconSpaceInvaders, iconSchoolboy, iconPackman;

void desktop()
{
    //joy.updatePositionX0Y0(0, 127, 0, 64);
    joy.updatePositionXY();

    gfx.print("Move the cursor\nto the Pong game\nshortcut", 5, 10, 8, 5);
    iconSapper.shortcut(sapper_bits, 0, 30, gamePong, joy.posX0, joy.posY0);
    iconSpaceInvaders.shortcut(spaceInvaders_bits, 32, 30, gameSpaceInvaders, joy.posX0, joy.posY0);
    iconSchoolboy.shortcut(gears2_bits, 64, 30, gameSchoolboy, joy.posX0, joy.posY0);
    iconPackman.shortcut(icon_MyNullApp_bits, 96, 30, gamePackman, joy.posX0, joy.posY0);
    
    crs.cursor(true, joy.posX0, joy.posY0);
}

void interfaceBoard()
{
    if ((stateGame1 == false) && (stateGame2 == false) && (stateGame3 == false) && (stateGame4 == false)) gfx.render(desktop);
    if (stateGame1 == true) gfx.render(gamePong); //Pong
    if (stateGame2 == true) gfx.render(gameSpaceInvaders); //AI
    if (stateGame3 == true) gfx.render(gameSchoolboy);
    if (stateGame4 == true) gfx.render(gamePackman);
}

void setup()
{
    gfx.initializationSystem();
}


void loop()
{
    interfaceBoard();
}
