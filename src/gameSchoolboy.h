#pragma once

#include <Arduino.h>
#include <U8g2lib.h> 
#include "mg.h"

extern Joystick joy;
extern Graphics gfx;
extern Cursor crs;

Button btn1, btn2, btn3, btn4;

bool stateGame3 = false;

struct schoolBoy
{
    short numberSchoolboy;
    short numberMove;
    bool state;
};

String arrSchoolboy[] =
    {
        "Pepperov Vova",
        "Aksenov Sergej",
        "Kirzhaev Dima",
        "Feoktistov ZHenya",
        "YUdanov Egor",
        "Rudnev Dima",
        "Lapshin Denis",
        "Kulagin Evgenij",
        "Butarev Lev",
        "Anisimov ZHenya",
        "SHelpakova Tanya",
        "Abajkin Gleb",
        "Loginov Andrej",
        "Haskhazyan Roma",
        "Bychkova Liza",
        "Korolev Ruslan",
        "Puzhaev Arsenij",
        "Pelih Bogdan",
        "SHevchenko Igor",
        "Vlasov Kolya"};

String arrMove[] =
    {
        "Passed",
        "Podnimi ruki i zahodi",
        "Ulybnis i zahodi",
        "Stancuj i zahodi",
        "Daj pyat i zahodi",
        "Propusti hod",
        "Nachni snachala",
        "Ty segodnya Bratyunya"};

schoolBoy sb[sizeof(arrSchoolboy)/sizeof(arrSchoolboy[1])];
int countSchoolboy = sizeof(arrSchoolboy)/sizeof(arrSchoolboy[1]);
int countArrMove = sizeof(arrMove)/sizeof(arrMove[1]);

bool flagSetupGameSchoolboy = false;
void setupGames()
{
    if (flagSetupGameSchoolboy == false)
    {
        randomSeed(analogRead(29));
        
        for (int a = 0; a < countSchoolboy; a++)
        {
            sb[a].numberSchoolboy = a;                       // определяем ребенка

            if (sb[a].state == false)
            {
                sb[a].numberMove = random(1, countArrMove);  // определяем для детей действия
            }
            
            if (stateGame3 == false) sb[a].state = false;    // все дети не прошли, пока что
        }

        flagSetupGameSchoolboy = true; stateGame3 = true;
    }
}

int i{};
void buttonAB()
{
    if (btn1.button("Go boy", 0, 40, joy.posX0, joy.posY0))
    {
        i += 1; 
        if (i > countSchoolboy) i = 0;
        
        delay(150);
    }

    if (btn3.button("Re-play", 30 ,55, joy.posX0, joy.posY0)) flagSetupGameSchoolboy = false;

    if (btn4.button("Passed", 40, 40, joy.posX0, joy.posY0))
    {
        sb[i].state = true;
        sb[i].numberMove = 0;
    }
    

    gfx.print(arrSchoolboy[i], 0, 10);
    gfx.print(arrMove[sb[i].numberMove], 0, 20);
    
    if (btn2.button("Exit", 0, 55, joy.posX0, joy.posY0))
    {
        stateGame3 = false; flagSetupGameSchoolboy = false; gfx.displayWait();
    }
    
}

void outputSerialSchoolboy()
{
    for (schoolBoy all : sb)
    {
        Serial.println(arrSchoolboy[all.numberSchoolboy]);
        Serial.println(arrMove[all.numberMove]);
        Serial.println("");
    }
}

void outputRenderSchoolboy()
{
    gfx.print("Please Push\nbutton A", 0, 10, 10, 5);
}

void gameSchoolboy()
{
    setupGames(); buttonAB(); //stateGame3 = true;

    joy.updatePositionX0Y0(0, 127, 0, 64);
    crs.cursor(true, joy.posX0, joy.posY0);
}