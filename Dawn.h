#pragma once
#ifndef DAWN_H
#define DAWN_H

#include "Rendering.h"
#include "Bullet.h"

enum Anim;

class SDL_Renderer;

class Dawn: public RenderSprite
{
    public:
        SDL_Renderer* gRenderer; //points to the global renderer in jill.cpp
        bool* ControlState;
        LTexture* gJillTexture;
        bool* HitBoxFlag;

        int framer();

        //Initializes the variables
        Dawn();

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e,const  Uint8* KeyStates );

        //Moves the dot
        void move();

        void GetCollisionVector(std::vector<CollisionLine> InputeCollisionVector);

        void Logic();
        //Shows the dot on the screen
        void render();
        void HitBoxRender();
        void AttackBoxRender();
    std::vector<SDL_Rect> GetHitBoxes();
    std::vector<SDL_Rect> GetAttackBoxes();
        void land();

        Anim GetAnim();

        // Position accessors in string format
    std::string getPosXString();
    std::string getPosYString();
    std::string getVertString();
        //		getPosXString()

        //Set the Cameras
        void SetCam(float cXpos, float cYpos);

        int getRenderPos(){
            return mPosY + 218;
        }

    std::vector<BulletCoord> GetJillBullets();

        void DestroyJillBullet(int index);

        int GetWidth();

        void DetectHit(std::vector<SDL_Rect> InRect, float InPosY);

        void MoveTrue();

        void MoveFalse();

    private:

        /*
        //The X and Y offsets of the dot
        float mPosX, mPosY;

        //The velocity of the dot
        float mVelX, mVelY;

        //Vertical distance
        float VertDis;

        //Vertical Velocity
        float VertVel;
        */

        bool TriggerBool;

        // The Current Animation
        Anim DawnAnim ;

        // FlipState
        bool IsFlipped;

        bool InAir;

        // Collision Detection
        SDL_Rect mCollider;

        SDL_Rect *Anim_Rect;
        // The animation frame
        int Frame;

        bool blinker;

        SDL_Rect ShadowRect;

        float ShadowHeight;

        bool anim_indicator;

        BulletHandler JillBullets;

    std::vector<attackboxes> DawnHitBoxes;
    std::vector<attackboxes> DawnAttackBoxes;
    std::vector<CollisionLine> DawnsCollisionVector;

        bool JillCheckCollision();

        int DAWN_HEIGHT = 100;
        int DAWN_WIDTH = 100;

        bool Dawn_Still = false;

        bool PunchForward = true;

        bool CheckCollisionLine(CollisionLine* JillsLine);

        bool IsWallCollidingX = false;

        CollisionLineType CurrentCollisionLine ;

        int AnimOffsetX = 0;

        int AnimOffsetY = 0;

        bool BattleStance = false;

        int DAWN_VEL = DAWN_WALK_VEL;

        bool IsHit = false;

        int HitCounter = 0;

        int ThisFrame = 0;

        SDL_Rect gDawnClips[60];

        bool IsMoving = false;

        bool IsMovingUP = false;
        bool IsMovingDOWN = false;
        bool IsMovingRIGHT = false;
        bool IsMovingLEFT = false;

};


#endif
