#include "Collision.h"
#include "Enums.h"
#include <iostream>
#include <fstream>

using namespace std;

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

vector<attackboxes>  CollisionBoxArray(string FileInput){
    vector<attackboxes> ReturnArray;
    attackboxes TempBoxes;
    BoxRect TempRect;
    ifstream InFile(FileInput);
    int temp = 0;
    int BoxIndex = 0;
    InFile >> temp;
    while(!InFile.eof()){
        InFile >> temp;
        if(temp != 1111){
            TempRect.mX = temp;
            for(int i = 0; i <3; i++){
                switch(i){
                    case 0: InFile>> TempRect.mY; break;
                    case 1: InFile>> TempRect.mW; break;
                    case 2: InFile>> TempRect.mH; break;
                }
            }
            TempBoxes.AttackRects.push_back(TempRect);
        }

        else{
            TempBoxes.Index = BoxIndex;
            BoxIndex++;
            ReturnArray.push_back(TempBoxes);
            TempBoxes.AttackRects.clear();

        }

    }
    return ReturnArray;
}

