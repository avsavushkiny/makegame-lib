#include <mg.h>

#define WIDTH 128 
#define HEIGHT 64
#define UNIT_WIDTH 5
#define UNIT_HEIGHT 4
#define STATUS_WIDTH 15
#define X 0
#define Y 1
#define ENEMY_Y_LIMIT WIDTH-(UNIT_WIDTH+5)*ENEMIES_COLS-STATUS_WIDTH-5

#define CTRL_TO 50
#define ENEMY_TO 1000
#define ROCKET_FLY_TO 50
#define ROCKET_FIRE_TO 1000
#define PLASMA_FLY_TO 50
#define PLASMA_FIRE_TO 1000

#define ENEMIES_COLS 5
#define ENEMIES_ROWS 3
#define ENEMIES_Q ENEMIES_COLS*ENEMIES_ROWS

Joystick joy;
Graphics gfx;
Interface inf;

struct unit {
    int state = 1; 
    int coords[2];
};

int pinLeft = A0;
int pinRight = A1;

unit player;
unit enemies[ENEMIES_Q];
unit rocket;
unit plasma;
unsigned long t, ctrl_next, enemy_next, rocket_fire_next, rocket_fly_next, 
plasma_fire_next, plasma_fly_next;

int enemy_drift_x = 0;
int enemy_drift_dir = 1;
int score = 0;
int life = 3;

int player_sprite[UNIT_WIDTH*UNIT_HEIGHT] = {
    0,0,1,0,0,
    0,1,1,1,0,
    1,1,0,1,1,
    1,1,0,1,1
};

int enemy_sprite[UNIT_WIDTH*UNIT_HEIGHT] = {
    1,0,1,0,1,
    0,1,1,1,0,
    0,1,0,1,0,
    1,0,1,0,1
};

//--------------------------------------------------------------------
void enemyMove(){
    if(enemy_drift_dir > 0 && enemy_drift_x < ENEMY_Y_LIMIT){
        enemy_drift_x ++;
    } 
    else if(enemy_drift_dir < 0 && enemy_drift_x > 5){
        enemy_drift_x --;
    }
    else{
        enemy_drift_dir = -1*enemy_drift_dir;
    }
    for(int e = 0; e < ENEMIES_Q; e++){
        enemies[e].coords[X] += enemy_drift_dir;
    }
}

void drawEnemyPlasma(){
    if (!plasma.state){
        return;
    }
    u8g2.drawBox(plasma.coords[X], plasma.coords[Y], 1, 3);
}

void fireEnemyPlasma(int idx){
    plasma.state = 1;
    plasma.coords[X] = enemies[idx].coords[X] + UNIT_WIDTH/2;
    plasma.coords[Y] = enemies[idx].coords[Y] + UNIT_WIDTH;
}

void PlasmaMove(){
    if(!plasma.state){
        return;
    }
    if(plasma.coords[X] > player.coords[X] && plasma.coords[X] < player.coords[X] + UNIT_WIDTH && plasma.coords[Y] > player.coords[Y]){
        playerKill();
        return;
    }
    if(plasma.coords[Y] == HEIGHT){
        plasma.state = 0;
    }
    plasma.coords[Y]++;
}

void enemyKill(int idx){
    rocket.state = 0;
    enemies[idx].state = 0;
    score++;
}
//-----

void handleControls(){
    if (player.coords[X] > 0 && player.coords[X] < WIDTH-1){
        player.coords[X] += joy.calculateIndexX0();
    }
}

void drawInterface(){
    gfx.print((String)score, WIDTH - STATUS_WIDTH, 1, 6, 4);
    gfx.print((String)life, WIDTH - STATUS_WIDTH, 20, 6, 4);
}

void drawPlayer(){
    for(int px = 0; px < UNIT_WIDTH; px++){
        for(int py = 0; py < UNIT_HEIGHT; py++){
            u8g2.drawXBMP(player.coords[X] + px, player.coords[Y] + py, player_sprite[px + py*UNIT_WIDTH]);
        }
    }
}

void drawEnemies(){
    for(int e = 0; e < ENEMIES_Q; e++){
        if(enemies[e].state){
            for(int px = 0; px < UNIT_WIDTH; px++){
                for(int py = 0; py < UNIT_HEIGHT; py++){
                    u8g2.drawXBMP(enemies[e].coords[X] + px, enemies[e].coords[Y] + py, enemy_sprite[px + py*UNIT_WIDTH]);
                }
            }
        }
    }
}

void drawPlayerRocket(){
    if(!rocket.state){
        return;
    }
    u8g2.drawBox(rocket.coords[X], rocket.coords[Y], 1, 1);
}
//-----
void firePlayerRocket(){
    rocket.state = 1;
    rocket.coords[X] = player.coords[X] + UNIT_WIDTH/2;
    rocket.coords[Y] = player.coords[Y] - 1;
}

void rocketMove(){
    if(!rocket.state){
        return;
    }
    for(int e = 0; e < ENEMIES_Q; e++){
        if(enemies[e].state && rocket.coords[X] > enemies[e].coords[X] && rocket.coords[X] < enemies[e].coords[X] + UNIT_WIDTH && rocket.coords[Y] > enemies[e].coords[Y] + UNIT_HEIGHT){
            enemyKill(e);
            return;
        }
    }
    if(rocket.coords[Y] == 0){
        rocket.state = 0;
    }
    rocket.coords[Y]--;
}

void playerKill(){
    plasma.state = 0;
    if(life>0){
        life--;
    }
}
//--------------------------------------------------------------------

void setup(){
    gfx.initializationSystem(); 
    player.coords[X] = WIDTH/2 - UNIT_WIDTH/2;
    player.coords[Y] = HEIGHT - UNIT_HEIGHT - 1;

    for(int x = 0; x < ENEMIES_COLS; x++){
        for(int y = 0; y < ENEMIES_ROWS; y++){
            int e = x + y*ENEMIES_COLS;
            enemies[e].coords[X] = 5 + x*(5+UNIT_WIDTH);
            enemies[e].coords[Y] = 5 + x*(3+UNIT_WIDTH);
        }
    }

    ctrl_next = millis() + CTRL_TO;
}

void game(){
    
}

void loop(){
    unsigned long t = millis();
    if(t > ctrl_next){
        ctrl_next = t + CTRL_TO;
        
        handleControls();

        u8g2.clearDisplay();
        drawInterface();
        drawPlayer();
        drawEnemies();
        drawEnemyPlasma();
        drawPlayerRocket();
        u8g2.display();
    }
    if(t > enemy_next){
        enemy_next = t + ENEMY_TO;
        enemyMove();
    }
    if(t > plasma_fire_next && !plasma.state){
        plasma_fire_next = t + PLASMA_FIRE_TO;
        int e;
        int idx = 255;
        for(int y=0; y<ENEMIES_ROWS; y++){
            for(int x=0; y<ENEMIES_COLS; x++){
                e = x+(2-y)*ENEMIES_COLS;
                if(enemies[e].state){
                    idx = e;
                    break;
                }
            }
            if(idx < 255){
                break;
            }
        }
        if(idx < 255){
            fireEnemyPlasma(idx);
        }
    }
    if(t > plasma_fire_next){
        plasma_fly_next = t + PLASMA_FLY_TO;
        PlasmaMove();
    }
    if(t > rocket_fire_next && !rocket.state){
        rocket_fire_next = t + ROCKET_FIRE_TO;
        firePlayerRocket();
    }
    if(t > rocket_fly_next){
        rocket_fly_next = t + ROCKET_FLY_TO;
        rocketMove();
    }
}