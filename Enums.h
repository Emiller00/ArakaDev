#pragma once
#ifndef ENUMS_H
#define ENUMS_H

enum ControllerButton{
    C_UP,
    C_DOWN,
    C_LEFT,
    C_RIGHT,
    C_START,
    C_BACK,
    C_JOYLEFT,
    C_JOYRIGHT,
    C_LB,
    C_RB,
    C_A,
    C_B,
    C_X,
    C_Y
};

enum Anim{
    DAWN_WALK,
    JILL_Run,
    DAWN_PUNCH,
    DAWN_Idle,
    JILL_Dash,
    Jill_Jump,
    JILL_Shoot,
    JILL_LIGHT_KICK,
    Enemy_Idle,
    Enemy_Shoot,
    Enemy_Hit,
    MELEE_ENEMY_IDLE,
    MELEE_ENEMY_WALK,
    MELEE_ENEMY_PUNCH,
    MELEE_ENEMY_REACT,
    DAWN_PRESS_DOWN,
    DAWN_PRESS_UP,
    DAWN_PRESS_LEFT,
    DAWN_PRESS_RIGHT,
    DAWN_COMBAT_IDLE,
    DAWN_BLOCK,
    DAWN_DAMAGE,
    DAWN_CROUCH,
    DAWN_CROUCHING,
    DAWN_UNCROUCHING
};

enum CollisionLineType{
    VERTICAL_LINE,
    HORIZONTAL_LINE_LOWER_STOP,
    HORIZONTAL_LINE_UPPER_STOP,
    DIAGONAL_LINE_RIGHT_STOP,
    DIAGONAL_LINE_LEFT_STOP,
};

enum CollisionType{
    UP_COLLISION,
    DOWN_COLLISION,
    RIGHT_COLLISION,
    LEFT_COLLISION,
    LEFT_DIAGONAL_COLLISION,
    RIGHT_DIAGONAL_COLLISION
};


enum Melee_Enemy_AI_Top_State{
    Melee_Enemy_Patrol,
    Melee_Enemy_Attack

};
enum Melee_Enemy_AI_Bottom_State{
    AI_Keeping_distance_Y,
    AI_Keeping_distance_X,
    AI_Idle,
    AI_Moving_In_Y,
    AI_Moving_In_X_Attacking,
    AI_Moving_In_Y_Attacking,
    AI_Punch
};

enum Melee_Enemy_AI_Move_State{
    AI_Walk_Down,
    AI_Walk_Up,
    AI_No_move,
    AI_Walk_Left,
    AI_Walk_Right,
    AI_Back_Left,
    AI_Back_Right

};
#endif
