#include "Collision.h"
#include "Enums.h"
#include <iostream>


CollisionLine::CollisionLine(const float BeginX,const float BeginY,const float InputLength,CollisionLineType InputType){

    ThisLine = InputType;

    LineLength = InputLength;

    BeginCoord.mX = BeginX;
    BeginCoord.mY = BeginY;

}

bool CollisionLine::DidCollide(float CheckX, float  CheckY, float CheckXNext, float CheckYNext){
    /*
       if(ThisLine == HORIZONTAL_LINE && CollisionCondition  == true){

       if(BeginCoord.mX<CheckX && CheckX < (BeginCoord.mX + LineLength) && CheckY < BeginCoord.mY){

       if
       return true;
       }

       }

       if(ThisLine == HORIZONTAL_LINE && CollisionCondition  == false){
       if(BeginCoord.mX<CheckX && CheckX < (BeginCoord.mX + LineLength) && CheckY > BeginCoord.mY){

       return true;
       }

       return false;
       }

       if(ThisLine == DIAGONAL_LINE &&CollisionCondition == true){

       }
       if(ThisLine == DIAGONAL_LINE &&CollisionCondition == false){

       }
       */

    return false;
}

void CollisionLine::RenderLine(SDL_Renderer* gRenderer, SDL_Rect InputRect){
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
    if((ThisLine == HORIZONTAL_LINE_LOWER_STOP)||(ThisLine == HORIZONTAL_LINE_UPPER_STOP)){
        SDL_RenderDrawLine(gRenderer,(BeginCoord.mX - InputRect.x  ),(BeginCoord.mY - InputRect.y ),(BeginCoord.mX - InputRect.x + LineLength),(BeginCoord.mY - InputRect.y ));
    }
    else if ((ThisLine == DIAGONAL_LINE_LEFT_STOP)||(ThisLine == DIAGONAL_LINE_RIGHT_STOP)){
        SDL_RenderDrawLine(gRenderer,(BeginCoord.mX - InputRect.x  ),(BeginCoord.mY - InputRect.y ),(BeginCoord.mX+LineLength*.7071067- InputRect.x),(BeginCoord.mY+LineLength*.7071067 - InputRect.y ));
    }

}

CollisionLineType CollisionLine::GetType(){
    return ThisLine;
}

CollisionCoord CollisionLine::GetLineCoord(){
    return BeginCoord;
}

float CollisionLine::GetLineLength(){

    return LineLength;

}

void CollisionLine::LineStats(){
    std::cout<<" This is a Collision line at "<<BeginCoord.mX<<" "<<BeginCoord.mY<<" Of Length "<<LineLength<< std::endl;

}
