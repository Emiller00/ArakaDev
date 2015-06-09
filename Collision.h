#pragma once
#ifndef COLLISION_H
#define COLLISION_H
#include <SDL.h>
#include <vector>
#include "Constants.h"

enum CollisionLineType;
#include "Structs.h"

class CollisionLine{
    public:
        // You declare the initial and Lengths of the Collision line
        // When you declare the class, you also specify if it is horizontal, vertical, or diagonal.

        CollisionLine(const float BeginX,const float BeginY,const float InputLength,CollisionLineType InputType);


        // Returns a bool if the line is out of bounds
        bool DidCollide(float CheckX, float CheckY, float CheckXNext, float CheckYNext);

        void RenderLine(SDL_Renderer* gRenderer, SDL_Rect InputRect);

        CollisionLineType GetType();

        float GetLineLength();

        CollisionCoord GetLineCoord();

        void LineStats();

    private:
        // The Beginning and ending coordinates

        CollisionCoord BeginCoord;
        // The type of line this is

        CollisionLineType ThisLine;

        // It can be either horizontal or diagonal
        //    CollisionType ThisType;
        // The Length of the line
        float LineLength;
        /* This boolean tells which side of the collision line you are
           For Vertical Lines
           True is right
           False is left
           For Diagonal Lines
           True is above
           False is Below
           For Horizontal Lines
           True is above
           False is below
           */
        bool CollisionCondition;

        std::vector<CollisionLine*> CoverBoxCollisionLines;
};


std::vector<attackboxes>  CollisionBoxArray(std::string FileInput);

#endif
