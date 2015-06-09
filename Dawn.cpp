#include "Dawn.h"
#include "Enums.h"

using namespace std;

Dawn::Dawn()
{
    //Initialize the offsets
    mPosX = 600;
    mPosY = 600;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    mCollider.w = DAWN_WIDTH;
    mCollider.h = DAWN_HEIGHT;

    //Initialize The Aniamtion
    DawnAnim = DAWN_Idle;

    // Set the Flip state and the other booleans
    IsFlipped = false;

    InAir = false;

    Frame = 0;

    //Vertical distance
    VertDis = 0;

    //Vertical Velocity
    VertVel = 0;

    blinker = true;

    ShadowRect.x =   0;
    ShadowRect.y =  400;
    ShadowRect.w =  100;
    ShadowRect.h = 50;
    TriggerBool = false;
    ShadowHeight = 0;

    anim_indicator = false;

    DawnHitBoxes = CollisionBoxArray("DawnHitInput.txt");
    DawnAttackBoxes = CollisionBoxArray("DawnAttackInput.txt");

    //Dawns Idle

    for(int i = 0; i<9;i++){
        gDawnClips[ i ].x =   i*FRAME_SIZE;
        gDawnClips[ i ].y =   0;
        gDawnClips[ i ].w =  FRAME_SIZE;
        gDawnClips[ i ].h = FRAME_SIZE;

    }
    //Dawn's walk cycle
    for(int i = 9; i < 17; i++){
        gDawnClips[ i ].x =   (i-9)*FRAME_SIZE;
        gDawnClips[ i ].y =   FRAME_SIZE;
        gDawnClips[ i ].w =  FRAME_SIZE;
        gDawnClips[ i ].h = FRAME_SIZE;
    }
    //Dawn's punch cycle
    for(int i = 18; i <23; i++){
        gDawnClips[ i ].x =   (i-18)*FRAME_SIZE;
        gDawnClips[ i ].y =   2*FRAME_SIZE;
        gDawnClips[ i ].w =  FRAME_SIZE;
        gDawnClips[ i ].h = FRAME_SIZE;
    }
    //Wall Presses
    for(int i = 24; i<28; i++){
        gDawnClips[ i ].x = (i-24)*FRAME_SIZE;
        gDawnClips[ i ].y =   3*FRAME_SIZE;
        gDawnClips[ i ].w =  FRAME_SIZE;
        gDawnClips[ i ].h = FRAME_SIZE;
    }
    // CombatIdle
    for(int i = 28; i<36; i++){
        gDawnClips[ i ].x = (i-28)*FRAME_SIZE;
        gDawnClips[ i ].y =   4*FRAME_SIZE;
        gDawnClips[ i ].w =  FRAME_SIZE;
        gDawnClips[ i ].h = FRAME_SIZE;
    }

    //Dawn Block

    for(int i = 36; i<38; i++){
        gDawnClips[ i ].x = (i-36)*FRAME_SIZE;
        gDawnClips[ i ].y =   5*FRAME_SIZE;
        gDawnClips[ i ].w =  FRAME_SIZE;
        gDawnClips[ i ].h = FRAME_SIZE;
    }

    //Dawn Damage

    for(int i = 38; i<43; i++){
        gDawnClips[ i ].x = (i-38)*FRAME_SIZE;
        gDawnClips[ i ].y =   6*FRAME_SIZE;
        gDawnClips[ i ].w =  FRAME_SIZE;
        gDawnClips[ i ].h = FRAME_SIZE;
    }

    for(int i = 43; i<55; i++){
        gDawnClips[ i ].x = (i-43)*FRAME_SIZE;
        gDawnClips[ i ].y =   7*FRAME_SIZE;
        gDawnClips[ i ].w =  FRAME_SIZE;
        gDawnClips[ i ].h = FRAME_SIZE;
    }

}

int Dawn::framer(){

    // This function determines how to animate Jill
    // If  you want to set the location of the animation
    // You must use the location* anim_speed
    AnimOffsetX = AnimOffsetY = 0;

    if(DawnAnim == DAWN_WALK){
        Frame ++;

        if(Frame/ANIM_SPEED>16||(Frame/ANIM_SPEED)<9){
            Frame =9*ANIM_SPEED;
            Dawn_Still = false;
        }
        return Frame/ANIM_SPEED;
    }
    else if(DawnAnim == DAWN_Idle){
        Frame++;

        if(Frame/ANIM_SPEED>8){
            Frame = 0;
        }
        return Frame/ANIM_SPEED;
    }
    else if(DawnAnim == Jill_Jump){
        Frame++;
        if((Frame/ANIM_SPEED)<18){
            Frame =18*ANIM_SPEED;
        }
        return Frame/ANIM_SPEED;
    }
    else if (DawnAnim == JILL_Shoot){
        Frame++;
        if(Frame/ANIM_SPEED>37||(Frame/ANIM_SPEED)<30){
            Frame =30*ANIM_SPEED;

        }

        if(Frame/ANIM_SPEED == 36){
            DawnAnim = DAWN_Idle;
        }
        return Frame/ANIM_SPEED;
    }
    else if (DawnAnim == DAWN_PUNCH){
        Dawn_Still = true;
        if(PunchForward){
            Frame++;
        }
        if(Frame/ANIM_SPEED<18||(Frame/ANIM_SPEED)>23){
            Frame =18*ANIM_SPEED;
        }

        if((Frame/ANIM_SPEED == 23)&&(PunchForward)){
            DawnAnim = DAWN_COMBAT_IDLE;
            BattleStance = true;
            Dawn_Still = false;

            PunchForward = true;
        }

        /*        if((Frame/ANIM_SPEED == 21)&&(!PunchForward)){
                  DawnAnim = DAWN_Idle;
                  Dawn_Still = false;
                  Frame = 0;
                  PunchForward = true;
                  }
                  */
        return Frame/ANIM_SPEED;
    }

    else if (DawnAnim == DAWN_PRESS_DOWN){

        Frame = 25*ANIM_SPEED;

        return Frame/ANIM_SPEED;

    }
    else if (DawnAnim == DAWN_PRESS_UP){

        Frame = 24*ANIM_SPEED;

        return Frame/ANIM_SPEED;

    }
    else if (DawnAnim == DAWN_PRESS_LEFT){

        Frame = 26*ANIM_SPEED;

        return Frame/ANIM_SPEED;
    }
    else if (DawnAnim == DAWN_PRESS_RIGHT){

        Frame = 27*ANIM_SPEED;

        return Frame/ANIM_SPEED;
    }

    else if(DawnAnim == DAWN_COMBAT_IDLE){
        AnimOffsetX = 50;
        Frame++;
        if(Frame/ANIM_SPEED>35||(Frame/ANIM_SPEED)<28){
            Frame =30*ANIM_SPEED;
        }

        return Frame/ANIM_SPEED;

    }
    else if(DawnAnim == DAWN_BLOCK){

        Frame++;
        if(ThisFrame>37||(ThisFrame)<36){
            Frame =36*ANIM_SPEED;
        }
        if(ThisFrame==37){
            if(BattleStance){
                DawnAnim = DAWN_COMBAT_IDLE;

            }
            else{
                DawnAnim = DAWN_Idle;

            }
        }

        return Frame/ANIM_SPEED;
    }
    else if(DawnAnim == DAWN_DAMAGE){

        Frame++;
        if(Frame/ANIM_SPEED>42||(Frame/ANIM_SPEED)<38){
            Frame =38*ANIM_SPEED;
        }
        if(ThisFrame==42){
            if(BattleStance){
                DawnAnim = DAWN_COMBAT_IDLE;

            }
            else{
                DawnAnim = DAWN_Idle;

            }
        }

        return Frame/ANIM_SPEED;
    }

    else if(DawnAnim == DAWN_CROUCHING){

        Frame++;
        if(Frame/ANIM_SPEED>55||(Frame/ANIM_SPEED)<43){

            Frame =43*ANIM_SPEED;

        }
        if(ThisFrame == 46){

            DawnAnim = DAWN_CROUCH;

        }

        return Frame/ANIM_SPEED;
    }
    else if(DawnAnim == DAWN_CROUCH){

        Frame++;
        if(Frame/ANIM_SPEED>54||(Frame/ANIM_SPEED)<43){

            Frame =46*ANIM_SPEED;

        }

        return Frame/ANIM_SPEED;
    }
    else if(DawnAnim == DAWN_UNCROUCHING){

        Frame--;
        if(Frame/ANIM_SPEED>46){

            Frame =46*ANIM_SPEED;

        }
        if(Frame/ANIM_SPEED ==42 ){
            Frame = 0;
            DawnAnim = DAWN_Idle;
            Dawn_Still = false;
            BattleStance = false;
        }

        return Frame/ANIM_SPEED;
    }
}

void Dawn::DetectHit(vector<SDL_Rect> InRect, float InPosY){
    SDL_Rect TempRect;
    SDL_Rect TempRect1 = InRect.at(0);
    SDL_Rect TempRect2 = GetHitBoxes().at(0);

    for(int i = 0; i<InRect.size();i++){
        TempRect1 = InRect.at(0);
        TempRect2 = GetHitBoxes().at(0);
        if(  SDL_IntersectRect(&TempRect2,&TempRect1,&TempRect)){
            if((IsFlipped&&mVelX>0)||(!IsFlipped&&mVelX<0)){

                DawnAnim = DAWN_BLOCK;

            }
            else{
                DawnAnim = DAWN_DAMAGE;
            }

        }

    }

    //  }
}

void Dawn::Logic(){
    //create a bullet once the animation shows Jill Shooting
    if(Frame == 900*ANIM_SPEED ){

        // anim_indicator = true;
        if(!IsFlipped){
            JillBullets.MakeBullet(mPosX+150,mPosY+20,false);

        }
        else{
            JillBullets.MakeBullet(mPosX,mPosY+20,true);

        }
    }

    for(int i = 0; i < JillBullets.GetBullets().size();i++){

        if(JillBullets.GetBullet(i).mX>2000||JillBullets.GetBullet(i).mX<0){

            JillBullets.DestroyBullet(i);
        }

    }

}

void Dawn::handleEvent( SDL_Event& e ,const Uint8* KeyStates  )
{

    //if( e.type == SDL_JOYBUTTONDOWN && e.key.repeat == 0)
    if( e.type == SDL_JOYBUTTONDOWN && e.jbutton.which == 0 )
    {

        // int TempInt = static_cast< int >(e.jbutton.button);
        // cout<< TempInt <<endl;

        switch( e.jbutton.button ){

            case C_UP:

                mVelX = mVelY = 0;
                mVelY -= DAWN_VEL;

                break;
            case C_DOWN:

                mVelX = mVelY = 0;
                mVelY += DAWN_VEL;

                break;
            case C_LEFT:

                mVelX = mVelY = 0;
                if(BattleStance&&!IsFlipped){
                    mVelX -= DAWN_BATTLE_VEL;
                }
                else{
                    mVelX -= DAWN_VEL;
                }

                if(mVelX<=0&&!BattleStance){
                    IsFlipped = true;

                }

                break;
            case C_RIGHT:

                mVelX = mVelY = 0;

                if(BattleStance&&IsFlipped){
                    mVelX += DAWN_BATTLE_VEL;
                }
                else{
                    mVelX += DAWN_VEL;
                }
                if(mVelX>=0&&!BattleStance){
                    IsFlipped = false;

                }

                break;
                //       case SDLK_SPACE: if(JillAnim != Jill_Jump){JillAnim = Jill_Jump; VertVel = JUMP_SPEED; InAir = true;} break;
                //       case SDLK_s: if (JillAnim != JILL_Shoot){JillAnim = JILL_Shoot;} break;
            case C_X: if (DawnAnim != DAWN_PUNCH&&DawnAnim != DAWN_CROUCHING){DawnAnim = DAWN_PUNCH;} break;
            case C_RB: if (DawnAnim != DAWN_CROUCHING){DawnAnim = DAWN_CROUCHING; Dawn_Still = true;} break;
            case C_A:

                           // if(mVelX != DAWN_VEL &&mVelY != DAWN_VEL){
                           BattleStance = !BattleStance;
                           if(!IsFlipped &&(mVelX<0)){
                               IsFlipped = !IsFlipped;
                               mVelX = -DAWN_VEL;
                           }
                           if(IsFlipped &&(mVelX>0)){
                               IsFlipped = !IsFlipped;
                               mVelX = DAWN_VEL;
                           }
                           // }
                           break;
        }

    }

    //If a key was released

    if( e.type == SDL_JOYBUTTONUP && e.jbutton.which == 0 )
    {
        //Adjust the velocity
        switch( e.jbutton.button )
        {

            case C_UP:if(mVelY == -DAWN_VEL){ mVelY += DAWN_VEL;IsMoving = IsMovingUP = false;} break;
            case C_DOWN:if(mVelY == DAWN_VEL){ mVelY -= DAWN_VEL;IsMoving = IsMovingDOWN = false;} break;
            case C_LEFT:if( mVelX == -DAWN_VEL){

                            mVelX += DAWN_VEL; IsMoving = IsMovingLEFT = false;
                        }
                        else if(mVelX == -DAWN_BATTLE_VEL){
                            mVelX += DAWN_BATTLE_VEL; IsMoving = IsMovingLEFT = false;
                        }

                        break;
            case C_RIGHT:if(mVelX ==DAWN_VEL){
                             mVelX -= DAWN_VEL;IsMoving = IsMovingRIGHT = false;
                         }
                         else if(mVelX == DAWN_BATTLE_VEL){
                             mVelX -= DAWN_BATTLE_VEL; IsMoving = IsMovingLEFT = false;
                         }
                         break;
            case C_RB: DawnAnim = DAWN_UNCROUCHING; break;

        }
        //}
}

}

void Dawn::move()
{

    if((DawnAnim != DAWN_BLOCK)&&(DawnAnim != DAWN_DAMAGE&&DawnAnim != DAWN_CROUCH&&DawnAnim != DAWN_CROUCHING&&DawnAnim != DAWN_UNCROUCHING)){

        //Move the dfot left or right
        if(*ControlState == true){
            mPosX += mVelX;//+mVelY*0.70710678118;
            mPosY += mVelY;//*0.70710678118;
        }

        else if (*ControlState == false){

            IsWallCollidingX = false;

            for (int i = 0; i<DawnsCollisionVector.size();i++){
                if(CheckCollisionLine(&DawnsCollisionVector.at(i))){
                    IsWallCollidingX = true;

                }

            }

            if(!Dawn_Still&&!IsWallCollidingX){
                mPosX += mVelX+mVelY*0.70710678118;

                mPosY += mVelY*0.70710678118;
            }
        }
        if(DawnAnim !=DAWN_PUNCH&&!BattleStance&&DAWN_CROUCH){
            DawnAnim = DAWN_Idle;
        }

        if(DawnAnim !=DAWN_PUNCH&&BattleStance&&DAWN_CROUCH){
            DawnAnim = DAWN_COMBAT_IDLE;
        }
        if(!BattleStance){
            if((CurrentCollisionLine == HORIZONTAL_LINE_LOWER_STOP) &&IsWallCollidingX){

                DawnAnim = DAWN_PRESS_DOWN;
            }
            if((CurrentCollisionLine == HORIZONTAL_LINE_UPPER_STOP) &&IsWallCollidingX){

                DawnAnim = DAWN_PRESS_UP;
            }
            if((CurrentCollisionLine == DIAGONAL_LINE_LEFT_STOP) &&IsWallCollidingX){

                DawnAnim = DAWN_PRESS_LEFT;
            }

            if((CurrentCollisionLine == DIAGONAL_LINE_RIGHT_STOP )&&IsWallCollidingX){

                DawnAnim = DAWN_PRESS_RIGHT;
            }
        }

        //If the dot went too far to the left or right

        if( ( mPosX < 0 ) || ( mPosX + DAWN_WIDTH > LEVEL_WIDTH ) )
        {
            //Move back
            mPosX -= mVelX;
        }

        if(InAir){
            VertVel-=GRAVITY;
        }
        VertDis+=VertVel;

        if(VertDis<0){
            land();

        }

        if(((mVelX)!= 0||(mVelY)!= 0)&&!(InAir)&&DawnAnim!=DAWN_PUNCH&&DawnAnim!=DAWN_PRESS_DOWN&&DawnAnim!=DAWN_PRESS_UP&&DawnAnim!=DAWN_PRESS_RIGHT&&DawnAnim!=DAWN_PRESS_LEFT){
            DawnAnim = DAWN_WALK;
        }
        //NOTE! This function will need to be changed everytime a new animation is added.
        // I meed to figure out a more elegant way to do this.

    }

    // Iterate the bullets
    for(int i = 0; i<JillBullets.NumBullets(); i++){
        if(JillBullets.GetBullet(i).flipped == false){
            JillBullets.BulletX(i,BULLET_SPEED);

        }
        else if(JillBullets.GetBullet(i).flipped == true){
            JillBullets.BulletX(i,-BULLET_SPEED);

        }

    }

}

void Dawn::GetCollisionVector(vector<CollisionLine> InputCollisionVector){

    // Must finish this later

    DawnsCollisionVector = InputCollisionVector;

}

int Dawn::GetWidth(){

    return DAWN_WIDTH;

}
void Dawn::render()
{

    // Set the Render Distance. This Takes into account the vertical offset (from jumping)
    int rendY = mPosY - VertDis - CposY;

    //blinker is a boolean that goes on and off
    //This function renders the shadow
    if(blinker){
        gJillTexture->render( gRenderer,mPosX - CposX, mPosY-CposY+110-ShadowHeight,DAWN_SCALE, &ShadowRect );

    }

    //Offset for the jumping animation
    if(DawnAnim == Jill_Jump){
        rendY -=30;
    }

    if(IsFlipped == false){
        /*       if(JillAnim == Jill_Jump){
                 gKunoTexture.render( mPosX - CposX, mPosY - CposY,JILL_SCALE, clip );
                 }
                 else{  */
        gJillTexture->render( gRenderer,mPosX - CposX+AnimOffsetX, rendY,DAWN_SCALE, Anim_Rect );

    }
    else if(IsFlipped == true){
        gJillTexture->render( gRenderer,mPosX - CposX-AnimOffsetX, rendY,DAWN_SCALE, Anim_Rect,0.0,NULL, SDL_FLIP_HORIZONTAL );
    }

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
    SDL_Rect fillRect = { mPosX - CposX+70, rendY+218, 80, 3};
    SDL_RenderFillRect( gRenderer, &fillRect );

    //render the bullets

    for(int i = 0 ; i < JillBullets.NumBullets(); i++){
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_Rect fillRect = { JillBullets.GetBullet(i).mX-CposX, JillBullets.GetBullet(i).mY-CposY, 10, 10};
        SDL_RenderFillRect( gRenderer, &fillRect );
        //           if(blinker){
        //           SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
        //            fillRect = { JillBullets.GetBullet(i).mX-CposX, JillBullets.GetBullet(i).mY-CposY+100, 5, 5};
        //           SDL_RenderFillRect( gRenderer, &fillRect );
        //   }

    }
    if(*HitBoxFlag){

        HitBoxRender();
        AttackBoxRender();
    }

    };

    void Dawn::MoveFalse(){

        IsMoving = false;

    }

    void Dawn::MoveTrue(){

        IsMoving = true;

    }

    void Dawn::AttackBoxRender(){
        int ScreenPosX = mPosX-CposX;
        int ScreenPosY = mPosY-CposY;
        float BoxX = 0;
        float BoxY = 0;
        float BoxW = 0;
        float BoxH = 0;
        int BoxFrame = Frame/ANIM_SPEED;
        for(int i = 0; i<DawnAttackBoxes.at(BoxFrame).AttackRects.size();i++){
            if (IsFlipped == false){

                BoxX = DawnAttackBoxes.at(BoxFrame).AttackRects.at(i).mX*DAWN_SCALE+ScreenPosX;
                BoxY = DawnAttackBoxes.at(BoxFrame).AttackRects.at(i).mY*DAWN_SCALE+ScreenPosY;
                BoxW = DawnAttackBoxes.at(BoxFrame).AttackRects.at(i).mW*DAWN_SCALE;
                BoxH = DawnAttackBoxes.at(BoxFrame).AttackRects.at(i).mH*DAWN_SCALE;
            }
            // I switched H and W
            //
            else if(IsFlipped == true){
                BoxX = ScreenPosX+335*DAWN_SCALE-DawnAttackBoxes.at(BoxFrame).AttackRects.at(i).mX*DAWN_SCALE-DawnAttackBoxes.at(BoxFrame).AttackRects.at(0).mW*DAWN_SCALE;
                BoxY = DawnAttackBoxes.at(BoxFrame).AttackRects.at(i).mY*DAWN_SCALE+ScreenPosY;
                BoxW = DawnAttackBoxes.at(BoxFrame).AttackRects.at(i).mW*DAWN_SCALE;
                BoxH = DawnAttackBoxes.at(BoxFrame).AttackRects.at(i).mH*DAWN_SCALE;
            }

            SDL_Rect fillRect = { BoxX, BoxY-VertDis, BoxW, BoxH};
            //      GetHitBoxes();

            SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0x7D );

            SDL_RenderFillRect( gRenderer, &fillRect );
        }
    }

    void Dawn::HitBoxRender(){
        int ScreenPosX = mPosX-CposX;
        int ScreenPosY = mPosY-CposY;
        float BoxX = 0;
        float BoxY = 0;
        float BoxW = 0;
        float BoxH = 0;
        int BoxFrame = Frame/ANIM_SPEED;
        for(int i = 0; i<DawnHitBoxes.at(BoxFrame).AttackRects.size();i++){
            if (IsFlipped == false){

                BoxX = DawnHitBoxes.at(BoxFrame).AttackRects.at(i).mX*DAWN_SCALE+ScreenPosX;
                BoxY = DawnHitBoxes.at(BoxFrame).AttackRects.at(i).mY*DAWN_SCALE+ScreenPosY;
                BoxW = DawnHitBoxes.at(BoxFrame).AttackRects.at(i).mW*DAWN_SCALE;
                BoxH = DawnHitBoxes.at(BoxFrame).AttackRects.at(i).mH*DAWN_SCALE;
            }
            // I switched H and W
            //
            else if(IsFlipped == true){
                BoxX = ScreenPosX+335*DAWN_SCALE-DawnHitBoxes.at(BoxFrame).AttackRects.at(i).mX*DAWN_SCALE-DawnHitBoxes.at(BoxFrame).AttackRects.at(0).mW*DAWN_SCALE;
                BoxY = DawnHitBoxes.at(BoxFrame).AttackRects.at(i).mY*DAWN_SCALE+ScreenPosY;
                BoxW = DawnHitBoxes.at(BoxFrame).AttackRects.at(i).mW*DAWN_SCALE;
                BoxH = DawnHitBoxes.at(BoxFrame).AttackRects.at(i).mH*DAWN_SCALE;
            }

            SDL_Rect fillRect = { BoxX, BoxY-VertDis, BoxW, BoxH};
            //      GetHitBoxes();

            SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0x7D );

            SDL_RenderFillRect( gRenderer, &fillRect );
        }
    }

    Anim Dawn::GetAnim(){

        return DawnAnim;

    }

    string Dawn::getPosXString(){

        return std::to_string(mPosX);
    }

    string Dawn::getPosYString(){

        return std::to_string(mPosY);
    }

    string Dawn::getVertString(){

        return std::to_string(VertDis);
    }

    void Dawn::land(){
        VertDis = 0;
        VertVel = 0;
        // JillAnim = JILL_Idle;
        InAir = false;

    }

    void Dawn::SetCam( float cXpos, float cYpos){

        CposX = cXpos;
        CposY = cYpos;
        ThisFrame = framer();
        Anim_Rect = &gDawnClips[ThisFrame];

    }

    vector<BulletCoord> Dawn::GetJillBullets(){

        return JillBullets.GetBullets();

    }

    bool Dawn::CheckCollisionLine(CollisionLine* JillsLine){

        if((JillsLine->GetType()==HORIZONTAL_LINE_LOWER_STOP)&&(mPosX+150>JillsLine->GetLineCoord().mX)&&(mPosX+70<(JillsLine->GetLineCoord().mX+JillsLine->GetLineLength()))){

            if((mVelY<0)&&(JillsLine->GetLineCoord().mY-305*DAWN_SCALE<mPosY)&&(JillsLine->GetLineCoord().mY-305*DAWN_SCALE>mPosY+mVelY*0.70710678118)){
                CurrentCollisionLine = HORIZONTAL_LINE_LOWER_STOP;

                return true;
            }

        }
        if((JillsLine->GetType()==HORIZONTAL_LINE_UPPER_STOP)&&(mPosX+150>JillsLine->GetLineCoord().mX)&&(mPosX+70<(JillsLine->GetLineCoord().mX+JillsLine->GetLineLength()))){

            if((mVelY>0)&&(JillsLine->GetLineCoord().mY-305*DAWN_SCALE>mPosY)&&(JillsLine->GetLineCoord().mY-305*DAWN_SCALE<mPosY+mVelY*0.70710678118)){
                CurrentCollisionLine = HORIZONTAL_LINE_UPPER_STOP;

                return true;
            }
        }
        if((JillsLine->GetType()==DIAGONAL_LINE_RIGHT_STOP)&&(mPosY+305*DAWN_SCALE>JillsLine->GetLineCoord().mY)&&((mPosY+305*DAWN_SCALE)<(JillsLine->GetLineCoord().mY+JillsLine->GetLineLength()/1.41421356237))){

            if((mVelX<0)&&(mPosY+305*DAWN_SCALE<mPosX+109*DAWN_SCALE+JillsLine->GetLineCoord().mY-JillsLine->GetLineCoord().mX)&&(mPosY+305*DAWN_SCALE-mVelX>mPosX+109*DAWN_SCALE+JillsLine->GetLineCoord().mY-JillsLine->GetLineCoord().mX)){

                CurrentCollisionLine = DIAGONAL_LINE_RIGHT_STOP;
                return true;

            }

        }

        if((JillsLine->GetType()==DIAGONAL_LINE_LEFT_STOP)&&(mPosY+305*DAWN_SCALE>JillsLine->GetLineCoord().mY)&&((mPosY+305*DAWN_SCALE)<(JillsLine->GetLineCoord().mY+JillsLine->GetLineLength()/1.41421356237))){

            if((mVelX>0)&&(mPosY+305*DAWN_SCALE>mPosX+220*DAWN_SCALE+JillsLine->GetLineCoord().mY-JillsLine->GetLineCoord().mX)&&(mPosY+305*DAWN_SCALE-mVelX<mPosX+220*DAWN_SCALE+JillsLine->GetLineCoord().mY-JillsLine->GetLineCoord().mX)){

                CurrentCollisionLine = DIAGONAL_LINE_LEFT_STOP;
                return true;

            }

        }

        return false;
    }

    bool Dawn::JillCheckCollision(){

        for(int i = 0; i<DawnsCollisionVector.size();i++){

        }
        return false; //josh: had to return something

    }

    vector<SDL_Rect> Dawn::GetHitBoxes(){
        int ScreenPosX = mPosX-CposX;
        int ScreenPosY = mPosY-CposY;
        vector<SDL_Rect> ReturnRectVect;
        SDL_Rect TempRect = {0,0,0,0};
        if (IsFlipped == false){
            for(int i = 0; i<DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.size();i++){
                BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i));
                TempRect.x = BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).x*DAWN_SCALE+ScreenPosX;
                TempRect.y = BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).y*DAWN_SCALE+ScreenPosY;
                TempRect.w = BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).w*DAWN_SCALE;
                TempRect.h = BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).h*DAWN_SCALE;
                ReturnRectVect.push_back(TempRect);

                TempRect = {0,0,0,0};
            }

        }
        else if(IsFlipped == true){
            for(int i = 0; i<DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.size();i++){
                BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i));
                TempRect.x = ScreenPosX+335*DAWN_SCALE-BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).x*DAWN_SCALE-BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).w*DAWN_SCALE;
                TempRect.y = BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).y*DAWN_SCALE+ScreenPosY;
                TempRect.w = BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).w*DAWN_SCALE;
                TempRect.h = BoxRectToSDL_Rect(DawnHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).h*DAWN_SCALE;
                ReturnRectVect.push_back(TempRect);

                TempRect = {0,0,0,0};
            }

        }

        return ReturnRectVect;
    }

    vector<SDL_Rect> Dawn::GetAttackBoxes(){
        int ScreenPosX = mPosX-CposX;
        int ScreenPosY = mPosY-CposY;
        vector<SDL_Rect> ReturnRectVect;
        SDL_Rect TempRect = {0,0,0,0};
        if (IsFlipped == false){
            for(int i = 0; i<DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.size();i++){
                BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i));
                TempRect.x = BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).x*DAWN_SCALE+ScreenPosX;
                TempRect.y = BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).y*DAWN_SCALE+ScreenPosY;
                TempRect.w = BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).w*DAWN_SCALE;
                TempRect.h = BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).h*DAWN_SCALE;
                ReturnRectVect.push_back(TempRect);

                TempRect = {0,0,0,0};
            }

        }
        else if(IsFlipped == true){
            for(int i = 0; i<DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.size();i++){
                BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i));
                TempRect.x = ScreenPosX+335*DAWN_SCALE-BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).x*DAWN_SCALE-BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).w*DAWN_SCALE;
                TempRect.y = BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).y*DAWN_SCALE+ScreenPosY;
                TempRect.w = BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).w*DAWN_SCALE;
                TempRect.h = BoxRectToSDL_Rect(DawnAttackBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).h*DAWN_SCALE;
                ReturnRectVect.push_back(TempRect);

                TempRect = {0,0,0,0};
            }

        }

        return ReturnRectVect;
    }

    void Dawn::DestroyJillBullet(int index){

        if(index !=9999){
            JillBullets.DestroyBullet(index);
        }
    }

