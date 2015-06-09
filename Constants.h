#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

const int LEVEL_WIDTH = 5000;
const int LEVEL_HEIGHT = 2000;
//Screen dimension constant
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int DEBUG_X = 600;
const int DEBUG_Y = 20;
const int IDLE_FRAMES = 9;
const int RUN_FRAMES = 9;
const int JUMP_FRAMES = 12;
const int  SHOOT_FRAMES = 8;
const int LIGHT_KICK_FRAMES = 5;
const float DAWN_SCALE = .7;
const float ENEMY_SCALE = 3;
const float BOX_SCALE = 1;
const float GRAVITY = 1;
const int FRAME_SIZE = 335;
const int ANIM_SPEED = 5;
const int ANIM_SPEED2 = 6;
const int ANIM_SPEED3 = 6;
const int JUMP_SPEED = 20;
const int NUM_BOXES = 3;
const int BULLET_SPEED = 15;
const int BGTILE_SIZE = 2000;
const int DAWN_WALK_VEL = 4;
const int DAWN_BATTLE_VEL = 3;
const int WALL_HEIGHT = 163;

// Animation Variable.
// Note the sign Convention
struct BoxRect{
    float mX;
    float mY;
    float mW;
    float mH;
};

struct attackboxes{
    int Index;
    std::vector<BoxRect> AttackRects;
    };

#endif
