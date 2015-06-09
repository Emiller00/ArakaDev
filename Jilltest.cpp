/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <vector>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Enums.h"
#include "Bullet.h"
#include "Structs.h"


//#include <SpriteBox.h>f
using namespace std;
//Level Dimensions

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

bool ControlState = false;
bool renderflag = false;
bool renderflag2 = false;
bool renderflag3 = false;
bool LineRenderFlag = false;
bool HitBoxFlag = false;
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
    vector<BoxRect> AttackRects;
    };


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

SDL_Rect BoxRectToSDL_Rect(BoxRect InRect){
    SDL_Rect OutRect;

   OutRect.x = InRect.mX;
   OutRect.y = InRect.mY;
   OutRect.h = InRect.mH;
   OutRect.w = InRect.mW;

   return OutRect;

}

//split Anim
// Into Action
// and Task

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, float scaler, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	protected:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};
class CollisionLine{
    public:
    // You declare the initial and Lengths of the Collision line
    // When you declare the class, you also specify if it is horizontal, vertical, or diagonal.

    CollisionLine(const float BeginX,const float BeginY,const float InputLength,CollisionLineType InputType);


    // Returns a bool if the line is out of bounds
    bool DidCollide(float CheckX, float CheckY, float CheckXNext, float CheckYNext);

    void RenderLine(SDL_Rect InputRect);

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

    vector<CollisionLine*> CoverBoxCollisionLines;
};

class RenderSprite{

public:

 virtual void SetCam(float cXpos, float cYpos) = 0;
 virtual void render() = 0;
 virtual int getRenderPos() = 0;
float getPosX(){
        return mPosX;
    }
float getPosY(){
        return mPosY;
	}
protected:

//The X and Y offsets of the dot
		float mPosX, mPosY;

		//The velocity of the dot
		float mVelX, mVelY;

        //Vertical distance
        float VertDis;

        //Vertical Velocity
        float VertVel;

        int CposX;
        int CposY;


};

class OcclusionTile: public RenderSprite{

public:
        OcclusionTile(SDL_Rect inRect, LTexture* inTecture, vector<CollisionLine> &CollisionlineVector);

        void render();

        void SetCam(float cXpos, float cYpos);

        int getRenderPos(){ return WALL_HEIGHT+mPosY;}

        void GetCoords(SDL_Rect CamRect, float InPosX, float InPosY);

private:
        SDL_Rect TileRect;

        LTexture* ThisTexture = nullptr;

        int ThisAlpha;

        float inPosX = 0;
        float inPosY = 0;
        SDL_Rect camRect;

};






class LBGTile: public LTexture
    {

    public:
        SDL_Rect GetRect();
        void SetRect(SDL_Rect InRect);
        void render(float scaler, SDL_Rect* clip = NULL,SDL_Rect* InputScreen = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    private:
        SDL_Rect BGRect;
    };


class LTimer
{
    private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;

    public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();
};




class CoverBox: public RenderSprite{

    public:

    CoverBox(float initX, float initY, vector<CollisionLine> &CollisionlineVector);
    CoverBox(float initX, float initY);
    void SetPos(float PosX, float PosY);

    void render();

    void SetCam( float cXpos, float cYpos);

    int getRenderPos(){

    return mPosY + 138;

    }

    vector<CollisionLine> GetCollisionLines();

    private:

    SDL_Rect BoxRect;
    vector<CollisionLine> CoverBoxCollisionLines;

};

class Dawn: public RenderSprite
{
    public:

        int framer();


		//Initializes the variables
		Dawn();


		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e,const  Uint8* KeyStates );

		//Moves the dot
		void move();


        void GetCollisionVector(vector<CollisionLine> InputeCollisionVector);

        void Logic();
		//Shows the dot on the screen
		void render();
        void HitBoxRender();
        void AttackBoxRender();
        vector<SDL_Rect> GetHitBoxes();
        vector<SDL_Rect> GetAttackBoxes();
        void land();

		Anim GetAnim();



		// Position accessors in string format
        string getPosXString();
        string getPosYString();
        string getVertString();
        //		getPosXString()

        //Set the Cameras
        void SetCam(float cXpos, float cYpos);



        int getRenderPos(){
            return mPosY + 218;
        }




        vector<BulletCoord> GetJillBullets();

        void DestroyJillBullet(int index);



        int GetWidth();

        void DetectHit(vector<SDL_Rect> InRect, float InPosY);

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

        vector<attackboxes> DawnHitBoxes;
        vector<attackboxes> DawnAttackBoxes;
        vector<CollisionLine> DawnsCollisionVector;

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



class Enemy: public RenderSprite
{
    public:

        int Framer();

		//The dimensions of the dot
		static const  int ENEMY_WIDTH = 100;
		static const   int ENEMY_HEIGHT = 100;

		//Maximum axis velocity of the dot
        const float ENEMY_WALK_VEL = 10;

		//Initializes the variables
		Enemy();


		//Takes key presses and adjusts the dot's velocity
	//	void handleEvent( SDL_Event& e,const  Uint8* KeyStates );

		//Moves the Enemy
		void move();

		//Shows the dot on the screen
		void render();

		Anim GetAnim();



		// Position accessors in string format
        string getPosXString();
        string getPosYString();
        string getVertString();
//		getPosXString()

        //Set the Cameras
        void SetCam(float cXpos, float cYpos, SDL_Rect* InputRect);



        int getRenderPos(){
            return mPosY + ENEMY_HEIGHT;
        }


        int EnemyLogic( vector<BulletCoord> PlayerBullets);


        private:


        bool TriggerBool;

		// The Current Animation
		Anim EnemyAnim ;

		// FlipState
		bool IsFlipped;

        bool InAir;

		// Collision Detection
		SDL_Rect mCollider;

        SDL_Rect* Anim_Rect;
		// The animation frame
		int Frame;

        bool blinker;



        SDL_Rect ShadowRect;

     //   Enemy_AI_State Top_AI_State;
     //   Enemy_AI_State Bottom_AI_State;
        bool anim_indicator;

        BulletHandler EnemyBullets;
        Melee_Enemy_AI_Move_State Move_State;

};

class MeleeEnemy: public RenderSprite{
public:



        MeleeEnemy(float inPosX, float inPosY, bool AttackModeOn);


		//The dimensions of the dot
		static const  int ENEMY_WIDTH = 100;
		static const   int ENEMY_HEIGHT = 100;

		//Maximum axis velocity of the dot
        const float ENEMY_WALK_VEL = DAWN_WALK_VEL;


		//Takes key presses and adjusts the dot's velocity
	//	void handleEvent( SDL_Event& e,const  Uint8* KeyStates );

		//Moves the Enemy
		void move();

		//Shows the dot on the screen
        void render();

        Anim GetAnim();



		// Position accessors in string format
        string getPosXString();
        string getPosYString();
        string getVertString();
//		getPosXString()

        //Set the Cameras
        void SetCam(float cXpos, float cY);



        int getRenderPos(){
            return mPosY + 75*ENEMY_SCALE;
        }


        int EnemyLogic( vector<BulletCoord> PlayerBullets);
        void HitBoxRender();
        void DetectHit(vector<SDL_Rect> InRects, float InPosY);
        vector<SDL_Rect> GetHitBoxes();
        vector<SDL_Rect> GetAttackBoxes();
        void AI(float InPosX, float InPosY);
        void GetPatrol(vector<PatrolPoint> InPatrolPointVector);
        void GetAnimRects(SDL_Rect InRects[]);
        void GetCollisionVector(vector<CollisionLine> InputCollisionVector);
        private:

        int Framer();

        bool TriggerBool;

		// The Current Animation
		Anim MeleeEnemyAnim ;

		// FlipState
		bool IsFlipped;

		// Collision Detection
		SDL_Rect mCollider;

        SDL_Rect *Anim_Rect;
		// The animation frame
		int ReturnFrame = 0;
		int  Frame =0;

        bool blinker;

        SDL_Rect ShadowRect;



        bool anim_indicator;

        BulletHandler EnemyBullets;

        vector<attackboxes> EnemyHitBoxes;
        vector<attackboxes> EnemyAttackBoxes;
        int HitCounter = 0;

        Melee_Enemy_AI_Top_State Top_AI_State;
        Melee_Enemy_AI_Bottom_State Bottom_AI_State;
        Melee_Enemy_AI_Move_State Move_State;
        vector<PatrolPoint> PatrolPointVector;
        PatrolPoint* NavPoint = nullptr;

        bool CloseDistanceY(int threshhold, int InPosY);
        bool KeepDistanceX(int threshhold, int InPosX, int OffsetX);
        bool GainDistanceX(int threshhold, int InPosX);
        bool ThrowPunch = false;
        bool EndPunch = false;
        SDL_Rect gMeleeEnemyClips[16];

        int ThisFrame = 0;

        int OffSetArray[16];
        int OffSetArrayFlipped[16];
        int KeepingXDistanct = 0;
        bool AttackModeON = true;
        vector<CollisionLine> MeleeEnemyCollisionVector;
        bool CheckCollisionLineX(int threshold,CollisionLine* MeleeEnemyLine);
        bool CheckCollisionLineY(int threshold,CollisionLine* MeleeEnemyLine);
        bool IsWallCollidingX = false;
        bool IsWallCollidingY = false;
};

// This class will make make an object that will check if a coordinate has collided with it
// This will primarily be used so that sprites don't go outside the bounds of the level


void printF(char *c, int x, int y);

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL jmatthew@aip.org
void close();

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gJillTexture;
LTexture gShadowTexture;
LTexture gGunEnemyTexture;
LTexture gMeleeEnemyTexture;
LTexture gBoxTexture;
LTexture gWall1;
LTexture gWall2;
LTexture gRoom1;
LTexture gBGTextures;
LTexture gPurpWall1;
//LTexture gBGTexture;


//Background Tile array

LTexture gTextTexture;
// SDL_Surface *DebugText = NULL;
//SDL_Color textColor = { 0, 0, 0 };
const int JILL_ANIMATION_FRAMES = IDLE_FRAMES + RUN_FRAMES + JUMP_FRAMES + SHOOT_FRAMES+LIGHT_KICK_FRAMES;
const int ENEMY_ANIMATION_FRAMES = 15;


SDL_Rect gEnemyClips[ ENEMY_ANIMATION_FRAMES+1];

SDL_Rect gBoxClips;
//Globally used font
TTF_Font *gFont = NULL;

//Surface

SDL_Surface* fontSurface;
SDL_Surface* screen;
SDL_Color TextColor = { 0, 0, 0, 0xFF };

SDL_Rect fontRect;


//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;



LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y,float scaler, SDL_Rect* clip , double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w*scaler;
		renderQuad.h = clip->h*scaler;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

SDL_Rect LBGTile::GetRect(){

    return BGRect;

}

void LBGTile::SetRect(SDL_Rect InRect){

    BGRect = InRect;

}

// MUST MODIFY
void LBGTile::render(float scaler, SDL_Rect* clip ,SDL_Rect* InputScreen , double angle , SDL_Point* center , SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
    SDL_Rect renderQuad;
    int ScreenX = InputScreen->x;
    int ScreenY = InputScreen->y;

	if(((BGRect.x -ScreenX)<=0 )&&((BGRect.y - ScreenY)<=0))
    {
        renderQuad = { 0, 0, mWidth, mHeight };


    }
    else if(((BGRect.x -ScreenX)<=0 )&&((BGRect.y - ScreenY)>0))
    {
        renderQuad = { 0, BGRect.y - ScreenY, mWidth, mHeight };


    }
    else if(((BGRect.x -ScreenX)>0 )&&((BGRect.y - ScreenY)<=0))
    {
        renderQuad = { BGRect.x -ScreenX, 0, mWidth, mHeight };


    }
	else{
        renderQuad = { BGRect.x -ScreenX, BGRect.y - ScreenY, mWidth, mHeight };

	}

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w*scaler;
		renderQuad.h = clip->h*scaler;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}


Enemy::Enemy()
{
    //Initialize the offsets
    mPosX = 1000;
    mPosY = 500;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    mCollider.w = ENEMY_WIDTH;
	mCollider.h = ENEMY_HEIGHT;

    //Initialize The Aniamtion
    EnemyAnim = Enemy_Idle;

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


    anim_indicator = false;

 //   Top_AI_State = Melee_Enemy_Patrol;
  //  Bottom_AI_State = AI_Idle;

}

MeleeEnemy::MeleeEnemy(float inPosX, float inPosY,bool AttackModeOn)
{
      //Initialize the offsets
    mPosX = inPosX;
    mPosY = inPosY-ENEMY_SCALE*75;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    mCollider.w = ENEMY_WIDTH;
	mCollider.h = ENEMY_HEIGHT;

    //Initialize The Aniamtion
    MeleeEnemyAnim = Enemy_Idle;

    // Set the Flip state and the other booleans
    IsFlipped = false;

            //Vertical distance
    VertDis = 0;

        //Vertical Velocity
    VertVel = 0;

    blinker = true;

    ShadowRect.x =  0;
    ShadowRect.y =  400;
    ShadowRect.w =  100;
    ShadowRect.h =  50;
    TriggerBool = false;


    anim_indicator = false;

    Top_AI_State = Melee_Enemy_Patrol;
    Bottom_AI_State = AI_Keeping_distance_Y;
    Move_State = AI_No_move;
    EnemyHitBoxes = CollisionBoxArray("MeleeEnemyInput.txt");
     EnemyAttackBoxes = CollisionBoxArray("MeleeEnemyAttackInput.txt");
          gMeleeEnemyClips[0].x = 0;
          gMeleeEnemyClips[0].y = 0;
          gMeleeEnemyClips[0].w = 80;
          gMeleeEnemyClips[0].h = 80;

          gMeleeEnemyClips[1].x = 80;
          gMeleeEnemyClips[1].y = 0;
          gMeleeEnemyClips[1].w = 80;
          gMeleeEnemyClips[1].h = 80;

          gMeleeEnemyClips[2].x = 80*2;
          gMeleeEnemyClips[2].y = 0;
          gMeleeEnemyClips[2].w = 80;
          gMeleeEnemyClips[2].h = 80;

          gMeleeEnemyClips[3].x = 80*3;
          gMeleeEnemyClips[3].y = 0;
          gMeleeEnemyClips[3].w = 80;
          gMeleeEnemyClips[3].h = 80;

          gMeleeEnemyClips[4].x = 80*4;
          gMeleeEnemyClips[4].y = 0;
          gMeleeEnemyClips[4].w = 79;
          gMeleeEnemyClips[4].h = 80;

          gMeleeEnemyClips[5].x = 0;
          gMeleeEnemyClips[5].y = 79;
          gMeleeEnemyClips[5].w = 80;
          gMeleeEnemyClips[5].h = 80;
          OffSetArray[5] = 7;

          gMeleeEnemyClips[6].x = 80;
          gMeleeEnemyClips[6].y = 79;
          gMeleeEnemyClips[6].w = 100;
          gMeleeEnemyClips[6].h = 80;
          OffSetArray[6] = 70;
          OffSetArrayFlipped[6] = 0;

          gMeleeEnemyClips[7].x = 180;
          gMeleeEnemyClips[7].y = 79;
          gMeleeEnemyClips[7].w = 80;
          gMeleeEnemyClips[7].h = 80;

          gMeleeEnemyClips[8].x = 179;
          gMeleeEnemyClips[8].y = 79;
          gMeleeEnemyClips[8].w = 80;
          gMeleeEnemyClips[8].h = 80;

          gMeleeEnemyClips[10].x = 316;
          gMeleeEnemyClips[10].y = 79;
          gMeleeEnemyClips[10].w = 80;
          gMeleeEnemyClips[10].h = 80;

          AttackModeON = AttackModeOn;

}

void MeleeEnemy::render()
{
    int rendY = mPosY - CposY;

    if(blinker){
    gShadowTexture.render( mPosX - CposX-5, mPosY-CposY+150,ENEMY_SCALE, &ShadowRect );

    }
    else{

    }

    if(IsFlipped == false){

    gMeleeEnemyTexture.render( mPosX - CposX-OffSetArray[ThisFrame], rendY,ENEMY_SCALE, Anim_Rect );

    }
    else if(IsFlipped == true){
    gMeleeEnemyTexture.render( mPosX - CposX-OffSetArrayFlipped[ThisFrame], rendY,ENEMY_SCALE, Anim_Rect,0.0,NULL, SDL_FLIP_HORIZONTAL );

    }
    if(HitBoxFlag){

        HitBoxRender();

    }
        /*
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
    SDL_Rect fillRect = { mPosX - CposX, getRenderPos()-CposY, 300, 3};
    SDL_RenderFillRect( gRenderer, &fillRect );
    */
}
void MeleeEnemy::move(){
        mVelX = mVelY=0 ;
        switch(Move_State){

       case AI_Walk_Up: mVelY = -ENEMY_WALK_VEL; break;
       case AI_Walk_Down: mVelY = ENEMY_WALK_VEL; break;
       case AI_Walk_Left: mVelX = -ENEMY_WALK_VEL; break;
       case AI_Walk_Right: mVelX = ENEMY_WALK_VEL; break;
       case AI_Back_Left: mVelX = -ENEMY_WALK_VEL*.8; break;
       case AI_Back_Right: mVelX = ENEMY_WALK_VEL*.8; break;
       case AI_No_move: mVelX = mVelY=0 ; break;

    }
     IsWallCollidingX = false;
     IsWallCollidingY = false;
    for (int i = 0; i<MeleeEnemyCollisionVector.size();i++){

        if(CheckCollisionLineX(0,&MeleeEnemyCollisionVector.at(i))){
            IsWallCollidingX = true;

            }
        if(CheckCollisionLineY(0,&MeleeEnemyCollisionVector.at(i))){
            IsWallCollidingY = true;

            }

    }

    if(!IsWallCollidingX||!IsWallCollidingY){
    mPosX += mVelX+mVelY*0.70710678118;

    mPosY += mVelY*0.70710678118;
    }

        IsFlipped = (mVelX>0);
        if((Move_State == AI_Back_Right)||(Move_State == AI_Back_Left)){
            IsFlipped = !IsFlipped;
        }
}

int MeleeEnemy::EnemyLogic( vector<BulletCoord> PlayerBullets){



        for(int i = 0; i< PlayerBullets.size(); i++){

                    // ATTN:
                    // Adjust the bullet collision here
                    if((PlayerBullets.at(i).mX+10>mPosX+10)&&(PlayerBullets.at(i).mX<mPosX+100)&&((PlayerBullets.at(i).mY>mPosY-5)&&(PlayerBullets.at(i).mY<mPosY+35))){

                      MeleeEnemyAnim = MELEE_ENEMY_REACT;
                        return i;
                    }

    }

      if(MeleeEnemyAnim !=MELEE_ENEMY_REACT){

        if(mVelX+mVelY != 0&&!ThrowPunch){
                MeleeEnemyAnim = MELEE_ENEMY_WALK;
        }
        else if(ThrowPunch){
                MeleeEnemyAnim = MELEE_ENEMY_PUNCH;
        }
        else{

                MeleeEnemyAnim = MELEE_ENEMY_IDLE;

        }
      }
     return 9999;

}



void MeleeEnemy::SetCam( float cXpos, float cYpos){

    CposX = cXpos;
    CposY = cYpos;
    ThisFrame = Framer();
    Anim_Rect = &gMeleeEnemyClips[ThisFrame];
    blinker = !blinker;
}

int MeleeEnemy::Framer(){


          if(MeleeEnemyAnim == MELEE_ENEMY_IDLE){

                    Frame = 0;

                    return Frame;

				}
          else if(MeleeEnemyAnim == MELEE_ENEMY_WALK){
                    Frame++;

                    if(ThisFrame>4){
                       ThisFrame = Frame = 0;
                    }
                    return Frame/ANIM_SPEED;
                }

          else if(MeleeEnemyAnim == MELEE_ENEMY_PUNCH){

                        Frame++;
                            if(ThisFrame<5||ThisFrame>8){
                                Frame =5*ANIM_SPEED2;
                            }

                            if((Frame-ANIM_SPEED2/3)/ANIM_SPEED2 == 8){
                                MeleeEnemyAnim = MELEE_ENEMY_IDLE;

                                EndPunch = true;

                            }
                        return Frame/ANIM_SPEED2;
            }
            else if(MeleeEnemyAnim = MELEE_ENEMY_REACT){

                   Frame = 10*ANIM_SPEED;
                    HitCounter++;
                    if(HitCounter>20){
                        MeleeEnemyAnim = MELEE_ENEMY_IDLE;
                        HitCounter = 0;

                    }
                    return Frame/ANIM_SPEED;
            }

}

void MeleeEnemy::HitBoxRender(){

      int ScreenPosX = mPosX-CposX;
      int ScreenPosY = mPosY-CposY;
      float BoxX = 0;
      float BoxY = 0;
      float BoxW = 0;
      float BoxH = 0;
      int BoxFrame = ThisFrame;
      for(int i = 0; i<EnemyHitBoxes.at(BoxFrame).AttackRects.size();i++){
      if (IsFlipped == false){

      BoxX = EnemyHitBoxes.at(BoxFrame).AttackRects.at(i).mX*ENEMY_SCALE+ScreenPosX;
      BoxY = EnemyHitBoxes.at(BoxFrame).AttackRects.at(i).mY*ENEMY_SCALE+ScreenPosY;
      BoxW = EnemyHitBoxes.at(BoxFrame).AttackRects.at(i).mW*ENEMY_SCALE;
      BoxH = EnemyHitBoxes.at(BoxFrame).AttackRects.at(i).mH*ENEMY_SCALE;

      }
      else if(IsFlipped == true){
      BoxX = ScreenPosX+335*DAWN_SCALE-EnemyHitBoxes.at(BoxFrame).AttackRects.at(i).mX*ENEMY_SCALE-EnemyHitBoxes.at(BoxFrame).AttackRects.at(0).mW*ENEMY_SCALE;
      BoxY = EnemyHitBoxes.at(BoxFrame).AttackRects.at(i).mY*ENEMY_SCALE+ScreenPosY;
      BoxW = EnemyHitBoxes.at(BoxFrame).AttackRects.at(i).mW*ENEMY_SCALE;
      BoxH = EnemyHitBoxes.at(BoxFrame).AttackRects.at(i).mH*ENEMY_SCALE;

      }
      SDL_Rect fillRect = { BoxX, BoxY-VertDis, BoxW, BoxH};

      SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0x7D );

      SDL_RenderFillRect( gRenderer, &fillRect );
      }

}

void MeleeEnemy::DetectHit(vector<SDL_Rect> InRect, float InPosY){
    SDL_Rect TempRect;
    SDL_Rect TempRect1 = InRect.at(0);
    SDL_Rect TempRect2 = GetHitBoxes().at(0);

    if((InPosY>mPosY-10+10)&&(InPosY<mPosY+40+10)){

    for(int i = 0; i<InRect.size();i++){
    TempRect1 = InRect.at(0);
    TempRect2 = GetHitBoxes().at(0);
    if(  SDL_IntersectRect(&TempRect2,&TempRect1,&TempRect)){

        MeleeEnemyAnim = MELEE_ENEMY_REACT;
    }

    }

    }
}

void MeleeEnemy::GetPatrol(vector<PatrolPoint> InPatrolPointVector){

    PatrolPointVector = InPatrolPointVector;
    NavPoint = &PatrolPointVector.at(0);

}

bool MeleeEnemy::CloseDistanceY(int threshold,int InPosY){


                    if(InPosY>mPosY+20+threshold)  {
                                            Move_State = AI_Walk_Down;
                                                    return false;

                                                    }
                    else if(InPosY<mPosY+20-threshold) {
                                                Move_State = AI_Walk_Up;
                                                    return false;

                                                    }
                    else{
                        Move_State = AI_No_move;

                        return true;
                        }



}
bool MeleeEnemy::KeepDistanceX(int threshold,int InPosX, int OffsetX){

/*
                    if(InPosX>mPosX+50+threshold)  {
                                            Move_State = AI_Walk_Right;
                                                    return false;
                                                    }
                    else if(InPosX<mPosX+50-threshold) {
                                                Move_State = AI_Walk_Left;
                                                    return false;
                                                    }
*/
                    if(InPosX<mPosX){
                       if(InPosX+OffsetX+threshold<mPosX) {
                        Move_State = AI_Walk_Left;
                        return false;
                    }
                        else if(InPosX+OffsetX-threshold>mPosX){
                        Move_State = AI_Back_Right;
                        return false;
                        }
                        else{
                            Move_State = AI_No_move;
                            return true;
                        }
                    }
                     if((InPosX>mPosX)){

                       if(InPosX-OffsetX+threshold<mPosX) {
                        Move_State = AI_Back_Left;
                        return false;
                    }
                        else if(InPosX-OffsetX-threshold>mPosX){
                        Move_State = AI_Walk_Right;
                        return false;
                        }
                        else{
                            Move_State = AI_No_move;
                            return true;
                        }

                    }

                    else{
                        Move_State = AI_No_move;

                        return true;
                        }



}



void MeleeEnemy::AI(float InPosX, float InPosY){

switch(Top_AI_State){


    // In this case, the enemy
    case Melee_Enemy_Attack:

    if(MeleeEnemyAnim !=MELEE_ENEMY_REACT){
        switch(Bottom_AI_State){
      case AI_Keeping_distance_Y:
                if( CloseDistanceY(50, InPosY)||IsWallCollidingX){
                        Bottom_AI_State = AI_Keeping_distance_X;
                        Move_State = AI_No_move;

                  }

                    if((rand()%200 == 19)){

                    Bottom_AI_State = AI_Moving_In_Y_Attacking;
                    }

                    IsFlipped = (InPosX>mPosX);

           break;
        case AI_Keeping_distance_X:

            if(KeepDistanceX(10, InPosX,250+KeepingXDistanct)){
                KeepingXDistanct = rand()%100-50;
            };
            if((rand()%200 == 19)){

                    Bottom_AI_State = AI_Moving_In_Y_Attacking;
                    Move_State = AI_No_move;
            }
                    if(abs(InPosY-mPosY)>50){
                        Move_State = AI_No_move;
                         Bottom_AI_State = AI_Keeping_distance_Y;

                            }



                    IsFlipped = (InPosX>mPosX);
           break;
        case AI_Idle:

          break;
        case AI_Moving_In_Y_Attacking:
         IsFlipped = (InPosX>mPosX);
         if( CloseDistanceY(10, InPosY)){
                        Bottom_AI_State = AI_Moving_In_X_Attacking;

                  }
            break;
        case AI_Moving_In_X_Attacking:
                if(abs(InPosY-mPosY)>30){
                        Move_State = AI_No_move;
                         Bottom_AI_State = AI_Moving_In_Y_Attacking;

                                                    }

         else if( KeepDistanceX(10, InPosX,150)){

                          Bottom_AI_State = AI_Punch;

                                                    }


            break;

        case AI_Punch:
                IsFlipped = (InPosX>mPosX);
                if(!ThrowPunch){

                    ThrowPunch = true;



                }
                else if (EndPunch){

                    ThrowPunch = false;
                    EndPunch = false;
                    if(!( rand()%5 == 3)){
                        Bottom_AI_State = AI_Keeping_distance_X;
                    }
                    else{
                         Bottom_AI_State = AI_Moving_In_X_Attacking;
                    }
                }

        break;
        }
    }
    else{
        Move_State = AI_No_move;
    }
    break;

    case Melee_Enemy_Patrol:
        if(AttackModeON){
        if(abs(InPosY-20-mPosY)<100){
            if(((mVelX>0)&&(mPosX+450>InPosX)&&(mPosX<InPosX))||((mVelX<0)&&(mPosX-450<InPosX)&&(mPosX>InPosX))){
            renderflag2 = true;


                Top_AI_State = Melee_Enemy_Attack;


            }
            else{
                renderflag2 = false;
            }
        }

        else{
            renderflag2 = false;
        }
        }
        if(abs(NavPoint->mY-getRenderPos())<10){
        if(ENEMY_WALK_VEL/2>=abs(mPosX-NavPoint->mX)){

                NavPoint = NavPoint->NextPoint;

        }
        else if (mPosX >NavPoint->mX){
                Move_State = AI_Walk_Left;

        }
        else if (mPosX <NavPoint->mX){
                Move_State = AI_Walk_Right;

        }
        }
        else if(abs(NavPoint->mY-getRenderPos())>10){
        if(ENEMY_WALK_VEL/2>=abs(getRenderPos()-NavPoint->mY)){

                NavPoint = NavPoint->NextPoint;

        }
        else if (getRenderPos() >NavPoint->mY){
                Move_State = AI_Walk_Up;

        }
        else if (getRenderPos() <NavPoint->mY){
                Move_State = AI_Walk_Down;

        }
        }
       // cout<<NavPoint->mX<<" "<<NavPoint->mY<<endl;
    break;
        }

}

vector<SDL_Rect> MeleeEnemy::GetHitBoxes(){
    int ScreenPosX = mPosX-CposX;
    int ScreenPosY = mPosY-CposY;
    vector<SDL_Rect> ReturnRectVect;
    SDL_Rect TempRect = {0,0,0,0};
    if (IsFlipped == false){
    for(int i = 0; i<EnemyHitBoxes.at(ThisFrame).AttackRects.size();i++){
        BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i));
        TempRect.x = BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i)).x*ENEMY_SCALE+ScreenPosX;
        TempRect.y = BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i)).y*ENEMY_SCALE+ScreenPosY;
        TempRect.w = BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i)).w*ENEMY_SCALE;
        TempRect.h = BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i)).h*ENEMY_SCALE;
        ReturnRectVect.push_back(TempRect);


        TempRect = {0,0,0,0};
    }

    }
    else if(IsFlipped == true){
        for(int i = 0; i<EnemyHitBoxes.at(ThisFrame).AttackRects.size();i++){
        BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i));
        TempRect.x = ScreenPosX+335*DAWN_SCALE-BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i)).x*ENEMY_SCALE-BoxRectToSDL_Rect(EnemyHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).w*ENEMY_SCALE;
        TempRect.y = BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i)).y*ENEMY_SCALE+ScreenPosY;
        TempRect.w = BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i)).w*ENEMY_SCALE;
        TempRect.h = BoxRectToSDL_Rect(EnemyHitBoxes.at(ThisFrame).AttackRects.at(i)).h*ENEMY_SCALE;
        ReturnRectVect.push_back(TempRect);


        TempRect = {0,0,0,0};
    }


    }


    return ReturnRectVect;
}

vector<SDL_Rect> MeleeEnemy::GetAttackBoxes(){
    int ScreenPosX = mPosX-CposX;
    int ScreenPosY = mPosY-CposY;
    vector<SDL_Rect> ReturnRectVect;
    SDL_Rect TempRect = {0,0,0,0};
    if (IsFlipped == false){
    for(int i = 0; i<EnemyAttackBoxes.at(ThisFrame).AttackRects.size();i++){
        BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i));
        TempRect.x = BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i)).x*ENEMY_SCALE+ScreenPosX;
        TempRect.y = BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i)).y*ENEMY_SCALE+ScreenPosY;
        TempRect.w = BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i)).w*ENEMY_SCALE;
        TempRect.h = BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i)).h*ENEMY_SCALE;
        ReturnRectVect.push_back(TempRect);


        TempRect = {0,0,0,0};
    }

    }
    else if(IsFlipped == true){
        for(int i = 0; i<EnemyAttackBoxes.at(ThisFrame).AttackRects.size();i++){
        BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i));
        TempRect.x = ScreenPosX+335*DAWN_SCALE-BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i)).x*ENEMY_SCALE-BoxRectToSDL_Rect(EnemyHitBoxes.at(Frame/ANIM_SPEED).AttackRects.at(i)).w*ENEMY_SCALE;
        TempRect.y = BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i)).y*ENEMY_SCALE+ScreenPosY;
        TempRect.w = BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i)).w*ENEMY_SCALE;
        TempRect.h = BoxRectToSDL_Rect(EnemyAttackBoxes.at(ThisFrame).AttackRects.at(i)).h*ENEMY_SCALE;
        ReturnRectVect.push_back(TempRect);


        TempRect = {0,0,0,0};
    }


    }


    return ReturnRectVect;
}

void MeleeEnemy::GetCollisionVector(vector<CollisionLine> InputCollisionVector){

    // Must finish this later

    MeleeEnemyCollisionVector= InputCollisionVector;



}

bool MeleeEnemy::CheckCollisionLineY(int threshold, CollisionLine* MeleeEnemyLine){

    if((MeleeEnemyLine->GetType()==HORIZONTAL_LINE_LOWER_STOP)&&(mPosX+80*ENEMY_SCALE>MeleeEnemyLine->GetLineCoord().mX)&&(mPosX+20*ENEMY_SCALE<(MeleeEnemyLine->GetLineCoord().mX+MeleeEnemyLine->GetLineLength()))){

       if((mVelY<0)&&(MeleeEnemyLine->GetLineCoord().mY-80*ENEMY_SCALE<mPosY)&&(MeleeEnemyLine->GetLineCoord().mY-80*ENEMY_SCALE>mPosY+mVelY*0.70710678118)){


        return true;
       }


       }
    if((MeleeEnemyLine->GetType()==HORIZONTAL_LINE_UPPER_STOP)&&(mPosX+80*ENEMY_SCALE>MeleeEnemyLine->GetLineCoord().mX)&&(mPosX+20*ENEMY_SCALE<(MeleeEnemyLine->GetLineCoord().mX+MeleeEnemyLine->GetLineLength()))){

       if((mVelY>0)&&(MeleeEnemyLine->GetLineCoord().mY-80*ENEMY_SCALE>mPosY)&&(MeleeEnemyLine->GetLineCoord().mY-80*ENEMY_SCALE<mPosY+mVelY*0.70710678118)){


        return true;
       }
    }



        return false;
}

bool MeleeEnemy::CheckCollisionLineX(int threshold, CollisionLine* MeleeEnemyLine){


    if((MeleeEnemyLine->GetType()==DIAGONAL_LINE_RIGHT_STOP)&&(mPosY+80*ENEMY_SCALE>MeleeEnemyLine->GetLineCoord().mY)&&((mPosY+80*ENEMY_SCALE)<(MeleeEnemyLine->GetLineCoord().mY+MeleeEnemyLine->GetLineLength()/1.41421356237))){


       if((mVelX<0)&&(mPosY+80*ENEMY_SCALE<mPosX+20*ENEMY_SCALE+MeleeEnemyLine->GetLineCoord().mY-MeleeEnemyLine->GetLineCoord().mX)&&(mPosY+80*ENEMY_SCALE-mVelX>mPosX+20*ENEMY_SCALE+MeleeEnemyLine->GetLineCoord().mY-MeleeEnemyLine->GetLineCoord().mX)){


        return true;

       }

    }

    if((MeleeEnemyLine->GetType()==DIAGONAL_LINE_LEFT_STOP)&&(mPosY+80*ENEMY_SCALE>MeleeEnemyLine->GetLineCoord().mY)&&((mPosY+80*ENEMY_SCALE)<(MeleeEnemyLine->GetLineCoord().mY+MeleeEnemyLine->GetLineLength()/1.41421356237))){


       if((mVelX>0)&&(mPosY+80*ENEMY_SCALE>mPosX+80*ENEMY_SCALE+MeleeEnemyLine->GetLineCoord().mY-MeleeEnemyLine->GetLineCoord().mX)&&(mPosY+80*ENEMY_SCALE-mVelX<mPosX+80*ENEMY_SCALE+MeleeEnemyLine->GetLineCoord().mY-MeleeEnemyLine->GetLineCoord().mX)){


        return true;

       }

    }

        return false;
}

void Enemy::SetCam( float cXpos, float cYpos, SDL_Rect* InputRect){

    CposX = cXpos;
    CposY = cYpos;
    Anim_Rect = InputRect;
    blinker = !blinker;
}

void Enemy::render()
{
    int rendY = mPosY - VertDis - CposY;





    if(blinker){

        gShadowTexture.render( mPosX - CposX+27, mPosY-CposY+110,1.5, &ShadowRect );

    }

    else{

    }

 /*   if(JillAnim == Jill_Jump){
        rendY -=30;
    }
*/

    if(IsFlipped == false){
     /*       if(JillAnim == Jill_Jump){
            gKunoTexture.render( mPosX - CposX, mPosY - CposY,JILL_SCALE, clip );
            }
            else{  */
            gGunEnemyTexture.render( mPosX - CposX, rendY,ENEMY_SCALE, Anim_Rect );

    }
        else if(IsFlipped == true){
            gGunEnemyTexture.render( mPosX - CposX, rendY,ENEMY_SCALE, Anim_Rect,0.0,NULL, SDL_FLIP_HORIZONTAL );
            }


/*
    if(anim_indicator){
      SDL_Rect fillRect = { 500, 100, 50, 50};
      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );

      SDL_RenderFillRect( gRenderer, &fillRect );
    }
*/

        //render the bullets

    for(int i = 0 ; i < EnemyBullets.NumBullets(); i++){
                  SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                  SDL_Rect fillRect = { EnemyBullets.GetBullet(i).mX-CposX, EnemyBullets.GetBullet(i).mY-CposY, 10, 10};
                  SDL_RenderFillRect( gRenderer, &fillRect );
    //              if(blinker){
                 // SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
                //  fillRect = { JillBullets.GetBullet(i).mX-CposX, JillBullets.GetBullet(i).mY-CposY+100, 5, 5};
       //           SDL_RenderFillRect( gRenderer, &fillRect );
 //   }

                  }


    }

int Enemy::EnemyLogic( vector<BulletCoord> PlayerBullets){


    for(int i = 0; i< PlayerBullets.size(); i++){

                    // ATTN:
                    // Adjust the bullet collision here
                    if((PlayerBullets.at(i).mX+10>mPosX+10)&&(PlayerBullets.at(i).mX<mPosX+100)&&((PlayerBullets.at(i).mY>mPosY-5)&&(PlayerBullets.at(i).mY<mPosY+35))){

                      EnemyAnim = Enemy_Hit;
                        return i;
                    }

    }

    return 9999;
}
int Enemy::Framer(){


       if(EnemyAnim == Enemy_Idle){

            Frame = 0;
       }
       else if( EnemyAnim == Enemy_Shoot){
       Frame++;
                if(Frame/ANIM_SPEED>3||(Frame/ANIM_SPEED)<1){
                        Frame =1*ANIM_SPEED;
                        }
             if(Frame/ANIM_SPEED == 3){
                    EnemyAnim = Enemy_Idle;

       }
        }
       else if( EnemyAnim == Enemy_Hit){
            Frame++;
             if(Frame/ANIM_SPEED>5||(Frame/ANIM_SPEED)<4){
                        Frame =4*ANIM_SPEED;
                        }
             if(Frame/ANIM_SPEED == 5){
                    EnemyAnim = Enemy_Idle;

       }

}
return Frame/ANIM_SPEED;
}
void Enemy::move(){

    switch(Move_State){

       case AI_Walk_Up: mVelY += ENEMY_WALK_VEL; break;
       case AI_Walk_Down: mVelY -= ENEMY_WALK_VEL; break;
       case AI_Walk_Left: mVelX -= ENEMY_WALK_VEL; break;
       case AI_Walk_Right: mVelX += ENEMY_WALK_VEL; break;

    }
    mPosX += mVelX+mVelY*0.70710678118;

    mPosY += mVelY*0.70710678118;

    if(mVelY +mVelX  != 0 ){
        EnemyAnim = MELEE_ENEMY_WALK;

    }
}

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
    if(ControlState == true){
    mPosX += mVelX;//+mVelY*0.70710678118;
    mPosY += mVelY;//*0.70710678118;
    }

    else if (ControlState == false){

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
    gJillTexture.render( mPosX - CposX, mPosY-CposY+110-ShadowHeight,DAWN_SCALE, &ShadowRect );

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
            gJillTexture.render( mPosX - CposX+AnimOffsetX, rendY,DAWN_SCALE, Anim_Rect );

    }
    else if(IsFlipped == true){
    gJillTexture.render( mPosX - CposX-AnimOffsetX, rendY,DAWN_SCALE, Anim_Rect,0.0,NULL, SDL_FLIP_HORIZONTAL );
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
        if(HitBoxFlag){

            HitBoxRender();
            AttackBoxRender();
        }

}
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


CoverBox::CoverBox(float initX, float initY, vector<CollisionLine> &CollisionlineVector){

    BoxRect.x = 0;
    BoxRect.y = 0;
    BoxRect.h = 214;
    BoxRect.w = 214;

    mPosX = initX;
    mPosY = initY;
    VertDis = 0;
    VertVel = 0;
    mVelX = 0;
    mVelY = 0;
    CposX = 0;
    CposY = 0;
    float linelength = 145;
    float LineY = initY+130;
    float LineX = initX-5;
    CollisionLine Upline(LineX,LineY,linelength,HORIZONTAL_LINE_UPPER_STOP );
    CollisionLine LeftLine(LineX,LineY,linelength*.8666,DIAGONAL_LINE_LEFT_STOP);
    CollisionLine BottomLine(LineX+linelength*.6,LineY+linelength*.6,linelength,HORIZONTAL_LINE_LOWER_STOP);
    CollisionLine RightLine(LineX+linelength+7,LineY,linelength*.8666,DIAGONAL_LINE_RIGHT_STOP);

    CollisionlineVector.push_back(Upline);
    CollisionlineVector.push_back(LeftLine);
    CollisionlineVector.push_back(BottomLine);
    CollisionlineVector.push_back(RightLine);
}
CoverBox::CoverBox(float initX, float initY){

    BoxRect.x = 0;
    BoxRect.y = 0;
    BoxRect.h = 214;
    BoxRect.w = 214;

    mPosX = initX;
    mPosY = initY;
    VertDis = 0;
    VertVel = 0;
    mVelX = 0;
    mVelY = 0;
    CposX = 0;
    CposY =0;
    float linelength = 135;
    CollisionLine Upline(initX,initY+139,135,HORIZONTAL_LINE_UPPER_STOP );
    // CollisionLine LeftLine();
    // CollisionLine BottomLine();
    // CollisionLine RightLine();

    CoverBoxCollisionLines.push_back(Upline);

}
void Dawn::DestroyJillBullet(int index){

       if(index !=9999){
            JillBullets.DestroyBullet(index);
       }
}

void CoverBox::render(){

    //gBoxTexture.render( mPosX - CposX, mPosY - CposY,BOX_SCALE, &BoxRect,0.0,NULL, SDL_FLIP_HORIZONTAL );


    //if((0<(mPosY - CposY))&&((mPosY - CposY)<SCREEN_HEIGHT)&&(0<(mPosX - CposX))&&((mPosX - CposX)<SCREEN_WIDTH+107)){
    gBoxTexture.render(mPosX - CposX, mPosY - CposY, BOX_SCALE, &BoxRect);

 /*   SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
    SDL_Rect fillRect = { mPosX - CposX, mPosY - CposY+219, 300, 3};
    SDL_RenderFillRect( gRenderer, &fillRect );
 */
    //}
}

void CoverBox::SetCam( float cXpos, float cYpos){

    CposX = cXpos;
    CposY = cYpos;

}

vector<CollisionLine> CoverBox::GetCollisionLines(){

    return CoverBoxCollisionLines;

}


LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}


void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}
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



void CollisionLine::RenderLine(SDL_Rect InputRect){
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

    cout<<" This is a Collision line at "<<BeginCoord.mX<<" "<<BeginCoord.mY<<" Of Length "<<LineLength<<endl;

}

SDL_Rect Intersector(SDL_Rect ScreenRect, SDL_Rect BGRect){


//    cout<<ScreenRect.x<<" "<<ScreenRect.y<<" "<<ScreenRect.w<<" "<<ScreenRect.h<<" ";
  //  cout<<endl<<BGRect.x<<" "<<BGRect.y<<" "<<BGRect.w<<" "<<BGRect.h<<" ";
    SDL_Rect TempRect;
        TempRect.x = 0;
        TempRect.y = 0;
        TempRect.w = 0;
        TempRect.h = 0;
    int Rx = ScreenRect.x;
    int Ry = ScreenRect.y;
    int Rw = ScreenRect.w;
    int Rh = ScreenRect.h;

    int Sx = BGRect.x;
    int Sy = BGRect.y;
    int Sw = BGRect.w;
    int Sh = BGRect.h;

        if((Rx<Sx)&&(Sx<Rx+Rw)&&(Ry<Sy)&&(Sy<Ry+Rh)){
     //       cout<<"Upper left corner"<<endl;

            TempRect.x = 0;
            TempRect.y = 0;
            TempRect.w = Rx + Rw - Sx;
            TempRect.h = Ry + Rh - Sy;

        }
        else if(Sx<=Rx&&Rx<Sx+Sw-Rw&&Ry<Sy&&Sy<Ry+Rh){
       //     cout<<"Upper Edge"<<endl;
            TempRect.x = Rx-Sx;
            TempRect.y = 0;
            TempRect.w = Rw;
            TempRect.h = Ry + Rh - Sy;
        }
        else if(Sx+Sw-Rw<Rx&&Rx<Sx+Sw&&Ry<Sy&&Sy<Ry+Rh){
       //     cout<<"Upper Right Corner"<<endl;
            TempRect.x = Rx-Sx;
            TempRect.y = 0;
            TempRect.w = Sx+Sw-Rx;
            TempRect.h = Ry + Rh - Sy;

        }
        else if( (Rx<Sx)&&(Sx<Rx+Rw)&&(Sy<Ry)&&(Ry<Sy+Sh-Rh)){
    //        cout<<"Left Edge"<<endl;
            TempRect.x = 0;
            TempRect.y = Ry-Sy;
            TempRect.w = Rx+Rw-Sx;
            TempRect.h = Rh;
        }
        else if(Sx+Sw-Rw<Rx&&Rx<Sx+Sw&&Sy<Ry&&Ry<Sy+Sh-Rh){
     //       cout<<"Right Edge"<<endl;
                TempRect.x = Rx-Sx;
                TempRect.y = Ry-Sy;
                TempRect.w = Sx+Sw-(Rx);
                TempRect.h = Rh;
        }
        else if(Sx+Sw-Rw<Rx&&Rx<Sx+Sw&&Sy+Sh-Rh<Ry&&Ry<Sy+Sh){
       //     cout<<"Right Bottom Corner"<<endl;
                TempRect.x = Rx-Sx;
                TempRect.y = Ry-Sy;
                TempRect.w = Sx+Sw-(Rx);
                TempRect.h = Sy+Sh-(Ry);
        }
        else if(Sx<=Rx&&Rx<Sx+Sw-Rw&&Sy+Sh-Rh<Ry&&Ry<Sy+Sh){
        //    cout<<"Lower Edge"<<endl;
                TempRect.x = Rx-Sx;
                TempRect.y = Ry-Sy;
                TempRect.w = Rw;
                TempRect.h = Sy+Sh-(Ry);
        }
        else if(Rx<Sx&&Sx<Rx+Rw&&Sy+Sh-Rh<Ry&&Ry<Sy+Sh){
     //       cout<<"Lower Left Corner"<<endl;
            TempRect.x = 0;
            TempRect.y = Ry-Sy;
            TempRect.w = Rx+Rw-Sx;
            TempRect.h = Sy+Sh-Ry;
        }
         else if(Sx<=Rx&&Rx<Sx+Sw-Rw&&Sy<Ry&&Ry<Sh-Rh){
       //     cout<<"Inside"<<endl;
                TempRect.x = Rx-Sx;
                TempRect.y = Ry-Sy;
                TempRect.w = Rw;
                TempRect.h = Rh;
        }
        else {
        //    cout<<"Outside"<<endl;
                TempRect.x = 0;
                TempRect.y = 0;
                TempRect.w = 0;
                TempRect.h = 0;
        }
    return TempRect;

}

OcclusionTile::OcclusionTile(SDL_Rect inRect, LTexture* inTexture,vector<CollisionLine> &CollisionlineVector){

    mPosY = inRect.y;
    TileRect = inRect;
    float linelength = inRect.w - inRect.h+WALL_HEIGHT;
    float diagonallinelength = 1.41*(inRect.h-WALL_HEIGHT);
    ThisTexture = inTexture;
    ThisTexture->setBlendMode(SDL_BLENDMODE_BLEND);
    ThisAlpha = 255;
    CollisionLine Upline(inRect.x,inRect.y+WALL_HEIGHT, linelength,HORIZONTAL_LINE_UPPER_STOP );
    CollisionLine LeftLine(inRect.x,inRect.y+WALL_HEIGHT,diagonallinelength,DIAGONAL_LINE_LEFT_STOP);
    CollisionLine BottomLine(inRect.x+inRect.h-WALL_HEIGHT,inRect.y+inRect.h,linelength,HORIZONTAL_LINE_LOWER_STOP);
    CollisionLine RightLine(inRect.x+linelength,inRect.y+WALL_HEIGHT,diagonallinelength,DIAGONAL_LINE_RIGHT_STOP);

    CollisionlineVector.push_back(Upline);
    CollisionlineVector.push_back(LeftLine);
    CollisionlineVector.push_back(BottomLine);
    CollisionlineVector.push_back(RightLine);
}
void OcclusionTile::render(){
    ThisTexture->setAlpha(ThisAlpha);
    SDL_Rect InterSectRect;
    SDL_IntersectRect(&camRect,&TileRect,&InterSectRect);


    int inX = InterSectRect.x - camRect.x;
    int inY = InterSectRect.y - camRect.y;
    SDL_Rect RenderRect;
    RenderRect.x = InterSectRect.x - TileRect.x;
    RenderRect.y = InterSectRect.y - TileRect.y;
    RenderRect.w = InterSectRect.w;
    RenderRect.h = InterSectRect.h;

    // If the following line is true.
    // Than fade in the wall
    if((inPosY+305*DAWN_SCALE<TileRect.y+TileRect.h)&&((TileRect.x+TileRect.w)<109*DAWN_SCALE+inPosX)||(inPosY+305*DAWN_SCALE<TileRect.y+TileRect.h-WALL_HEIGHT)&&(((inPosY+305*DAWN_SCALE)<(inPosX+109*DAWN_SCALE+TileRect.y+TileRect.h-WALL_HEIGHT-(TileRect.x+TileRect.w))))||(inPosY+305*DAWN_SCALE>TileRect.y+TileRect.h||(((inPosY+305*DAWN_SCALE)>(inPosX+200*DAWN_SCALE+TileRect.y+WALL_HEIGHT-(TileRect.x)))))){


     if(ThisAlpha<255){  ThisAlpha+= 20;}
     else if (ThisAlpha>255){ThisAlpha = 255;}

    }
    else  {
        if(ThisAlpha>0){
        ThisAlpha -= 20;
        }
        else if (ThisAlpha<0){ThisAlpha = 0;}
    }
    if(ThisAlpha !=0){
        ThisTexture->render(inX,inY,1, &RenderRect );
    }
//    cout<<ThisAlpha<<endl;
}


void OcclusionTile::GetCoords( SDL_Rect CamRect, float InPosX, float InPosY){

    inPosX = InPosX;

    inPosY = InPosY;

    camRect = CamRect;
}

void OcclusionTile::SetCam(float cXpos, float cYpos){

}


bool init()
{
    //Generate a Random number
    srand (time(NULL));

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Enable VSync
		if( !SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" ) )
		{
			printf( "Warning: VSync not enabled!" );
		}

		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
        //Check for joysticks
		if( SDL_NumJoysticks() < 1 )
		{
			printf( "Warning: No joysticks connected!\n" );
		}
		else
		{
        //Load joystick
			gGameController = SDL_JoystickOpen( 0 );
			if( gGameController == NULL )
			{
				printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			}
		}
		//Create window
		gWindow = SDL_CreateWindow( "Jill2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
						 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{

	//Loading success flag
	bool success = true;


	// Kuno addition: load the kunoichi walk strip instead of the dot
	if( !gJillTexture.loadFromFile( "./DawnSprites2.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}


     	if( !gBGTextures.loadFromFile( "./SneakRoomFloor.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
        if( !gBoxTexture.loadFromFile( "./Box.png" ) )

	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
        if(!gShadowTexture.loadFromFile("./Shadow.png")){
        printf( "Failed to load background texture!\n" );
		success = false;
        }/*
        if(!gGunEnemyTexture.loadFromFile("./Enemy.png")){
        printf( "Failed to load background texture!\n" );
		success = false;
        }*/
        if(!gMeleeEnemyTexture.loadFromFile("./MeleeEnemy1.png")){
        printf( "Failed to load background texture!\n" );
		success = false;
        }

        if(!gPurpWall1.loadFromFile("./SneakRoomWall1.png")){
        printf( "Failed to load background texture!\n" );
		success = false;
        }
    else
	{



          //Define the rectangles for the Boxes
          gBoxClips.x = 0;
          gBoxClips.y = 0;
          gBoxClips.w = 107;
          gBoxClips.h = 107;
          //Define the rectangles for the Gun Enemy Enemy
          gEnemyClips[0].x = 0;
          gEnemyClips[0].y = 0;
          gEnemyClips[0].w = 80;
          gEnemyClips[0].h = 72;

          gEnemyClips[1].x = 0;
          gEnemyClips[1].y = 80;
          gEnemyClips[1].w = 80;
          gEnemyClips[1].h = 72;

          gEnemyClips[2].x = 80;
          gEnemyClips[2].y = 80;
          gEnemyClips[2].w = 90;
          gEnemyClips[2].h = 72;

          gEnemyClips[3].x =165;
          gEnemyClips[3].y = 80;
          gEnemyClips[3].w = 80;
          gEnemyClips[3].h = 72;

          gEnemyClips[4].x = 0;
          gEnemyClips[4].y = 180;
          gEnemyClips[4].w = 80;
          gEnemyClips[4].h = 66;

          gEnemyClips[5].x = 80;
          gEnemyClips[5].y = 180;
          gEnemyClips[5].w = 90;
          gEnemyClips[5].h = 66;


          SDL_Rect TempRect;
           TempRect.x = 0;
           TempRect.y = 0;
           TempRect.w = BGTILE_SIZE;
           TempRect.h = BGTILE_SIZE;



	}
	return success;
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void close()
{
	//Free loaded images
	gJillTexture.free();
    gShadowTexture.free();
    gGunEnemyTexture.free();
    gMeleeEnemyTexture.free();
    gRoom1.free();
    gWall1.free();
    gWall2.free();
    gBoxTexture.free();
    gTextTexture.free();
    gBGTextures.free();
    //Close game controller
    SDL_JoystickClose( gGameController );
    gGameController = NULL;
    //Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
    bool debug = false;

    SDL_Rect BGRenderRect;
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{


			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

            SDL_Color TextColor = { 0, 0, 0, 0xFF };

         //   gTextTexture.loadFromRenderedText( "geese", TextColor );

           // int frame = 11;

			/*

			OCCLUSION TILES
			Will have to reset these

			SDL_Rect Wall1Rect = {15,455,1597,1635};
			OcclusionTile Wall1(Wall1Rect,&gWall1);
			SDL_Rect Wall2Rect = {401,0,1136,865};
			OcclusionTile Wall2(Wall2Rect,&gWall2);
            SDL_Rect Room1Rect = {1337,619,2750,1485};
			OcclusionTile Room1(Room1Rect,&gRoom1);
			*/

			vector<CollisionLine> CollisionVector;

            SDL_Rect Purp1Rect = {2178,308-WALL_HEIGHT,1172,409};
			OcclusionTile oPurpWall1(Purp1Rect,&gPurpWall1,CollisionVector);
            SDL_Rect Purp2Rect = {2669,799-WALL_HEIGHT,1172,409};
			OcclusionTile oPurpWall2(Purp2Rect,&gPurpWall1,CollisionVector);
			Dawn jill;

            //   Enemy enemy1;
            MeleeEnemy enemy2(1935,235,true);
            PatrolPoint TempPoint;
            vector<PatrolPoint> TempPatrolPointVector;
            TempPatrolPointVector.push_back(TempPoint);
            TempPatrolPointVector.push_back(TempPoint);
            TempPatrolPointVector.push_back(TempPoint);
            TempPatrolPointVector.push_back(TempPoint);
            TempPatrolPointVector.at(0).NextPoint = &TempPatrolPointVector.at(1);
            TempPatrolPointVector.at(0).mX = 1900;
            TempPatrolPointVector.at(0).mY = 235;
            TempPatrolPointVector.at(1).NextPoint = &TempPatrolPointVector.at(2);
            TempPatrolPointVector.at(1).mX = 3239;
            TempPatrolPointVector.at(1).mY = 235;
            TempPatrolPointVector.at(2).NextPoint = &TempPatrolPointVector.at(3);
            TempPatrolPointVector.at(2).mX = 2360;
            TempPatrolPointVector.at(2).mY = 630;
            TempPatrolPointVector.at(3).NextPoint = &TempPatrolPointVector.at(0);
            TempPatrolPointVector.at(3).mX = 3600;
            TempPatrolPointVector.at(3).mY = 630;
            enemy2.GetPatrol(TempPatrolPointVector);


            /*
            CollisionLine  LineOne(0,2093,1620,HORIZONTAL_LINE_LOWER_STOP);

            CollisionLine  LineTwo(390,863,1740,DIAGONAL_LINE_RIGHT_STOP, true);
            CollisionLine  LineThree(390,865,1132,HORIZONTAL_LINE_LOWER_STOP, true);
            CollisionLine  LineFour(1038,375,690,DIAGONAL_LINE_RIGHT_STOP,true);
            CollisionLine  LineFive(1038,375,585,HORIZONTAL_LINE_LOWER_STOP,true);
            CollisionLine  LineSix(1615,375,690,DIAGONAL_LINE_LEFT_STOP,true);
            CollisionLine  LineSeven(2101,863,1550,HORIZONTAL_LINE_LOWER_STOP, true);
            CollisionLine  LineEight(1325,1030,1515,DIAGONAL_LINE_LEFT_STOP, true);
            CollisionLine  LineNine(1325,1030,1679,HORIZONTAL_LINE_UPPER_STOP, false);
            CollisionLine  LineTen(3015,1030,1515,DIAGONAL_LINE_RIGHT_STOP, true);
            CollisionLine  LineEleven(2400,2105,1679,HORIZONTAL_LINE_LOWER_STOP, true);
            // The vector that stores our collision lines

            CollisionVector.push_back(&LineOne);
            CollisionVector.push_back(&LineTwo);
            CollisionVector.push_back(&LineThree);
            CollisionVector.push_back(&LineFour);
            CollisionVector.push_back(&LineFive);
            CollisionVector.push_back(&LineSix);
            CollisionVector.push_back(&LineSeven);
            CollisionVector.push_back(&LineEight);
            CollisionVector.push_back(&LineNine);
            CollisionVector.push_back(&LineTen);
            CollisionVector.push_back(&LineEleven);
                        */
            int BulletToDestroy;


            CoverBox Box1(1902+2,557-135,CollisionVector);
            CoverBox Box2(1984+2,639-135,CollisionVector);
            CoverBox Box3(2066+2,721-135,CollisionVector);
            CoverBox Box4(2148+2,803-135,CollisionVector);
            CoverBox Box5(2230+2,885-135,CollisionVector);




                        // Our array of sprites pointers
            vector<RenderSprite*> RenderVector;

            RenderVector.push_back(&jill);
            RenderVector.push_back(&Box1);
            RenderVector.push_back(&Box2);
            RenderVector.push_back(&Box3);
            RenderVector.push_back(&Box4);
            RenderVector.push_back(&Box5);
            RenderVector.push_back(&oPurpWall1);
            RenderVector.push_back(&oPurpWall2);
            vector<CoverBox*> CoverBoxVector;
            CoverBoxVector.push_back(&Box1);
            CoverBoxVector.push_back(&Box2);
            CoverBoxVector.push_back(&Box3);
            CoverBoxVector.push_back(&Box4);
            CoverBoxVector.push_back(&Box5);
            //cout<<CoverBoxVector.at(0)->GetCollisionLines().at(0).GetCollisionCoord().mX;
        //  RenderVector.push_back(&enemy1);
            RenderVector.push_back(&enemy2);

            //cout<<CoverBoxVector.at(0)->GetCollisionLines().at(0)->GetLineCoord().mX<<endl;


        /*     for(int i = 0; i<CoverBoxVector.size();i++){

                for(int j = 0; j<CoverBoxVector.at(i)->GetCollisionLines().size();j++){

                    CollisionVector.push_back(CoverBoxVector.at(i)->GetCollisionLines().at(j));
                   // cout<<CoverBoxVector.at(0)->GetLineCoord().mX<<endl;
                }

            }
            */

          //  cout<<CollisionVector.at(0)->GetLineCoord().mX<<endl;


            jill.GetCollisionVector(CollisionVector);
            enemy2.GetCollisionVector(CollisionVector);

            int NumRenderSprites = RenderVector.size();
            RenderSprite* RenderArray[9999];

            for(int i = 0; i<NumRenderSprites; i++){
                RenderArray[i] = RenderVector.at(i);
            }


            //SDL camera
            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


			//While application is running
			while( !quit )
			{

            const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
                    else if( e.type == SDL_KEYDOWN)
					{
						//Handle backspace
						if(  e.key.keysym.sym == SDLK_d && SDL_GetModState() & KMOD_CTRL )
						{

                        debug = !debug;

						}
							if(  e.key.keysym.sym == SDLK_q && SDL_GetModState() & KMOD_CTRL )
						{

                        ControlState = !ControlState;

						}
						if(  e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
						{

                        HitBoxFlag = !HitBoxFlag;

						}
                        if(  e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
						{

                        LineRenderFlag = !LineRenderFlag;

						}

					}
					//Handle input for the dot
					// The kunoichi

					const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

					jill.handleEvent( e, currentKeyStates );
				}




				//Move the dot
				jill.move();
				jill.Logic();

                enemy2.move();
                enemy2.AI(jill.getPosX(),jill.getPosY());




                //     Dawn Destroy's a bullet if it hits an enemy
                //     jill.DestroyJillBullet(enemy1.EnemyLogic(jill.GetJillBullets()));
                jill.DestroyJillBullet(enemy2.EnemyLogic(jill.GetJillBullets()));

                //     Dawn and the enemy detect hitboxes

                enemy2.DetectHit(jill.GetAttackBoxes(),jill.getPosY());
                jill.DetectHit(enemy2.GetAttackBoxes(),enemy2.getPosY());

                //Center the camera over the dot
				camera.x = ( jill.getPosX() + jill.GetWidth() / 2 ) - SCREEN_WIDTH / 2;
				camera.y = ( jill.getPosY() + jill.GetWidth()/ 2 ) - SCREEN_HEIGHT / 2;

				//Keep the camera in bounds
				if( camera.x < 0 )
				{
					camera.x = 0;
				}
				if( camera.y < 0 )
				{
					camera.y = 0;
				}
				if( camera.x > LEVEL_WIDTH - camera.w )
				{
					camera.x = LEVEL_WIDTH - camera.w;
				}
				if( camera.y > LEVEL_HEIGHT - camera.h )
				{
					camera.y = LEVEL_HEIGHT - camera.h;
				}

                  // Display infor... I don't know if I should be loading every frame



            //    enemy1.SetCam(camera.x, camera.y, &gEnemyClips[enemy1.Framer()/ANIM_SPEED]);
            //    enemy2.SetCam(camera.x, camera.y);

				//Clear screen

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
               //  cout<<camera.y<<endl;
				//Render objects
              //  gBGTexture.render( 0, 0,1, &camera );


                gBGTextures.render(0,0,1,&camera);

                oPurpWall1.GetCoords(camera,jill.getPosX(),jill.getPosY());
                oPurpWall2.GetCoords(camera,jill.getPosX(),jill.getPosY());

    for(int i = 0; i< NumRenderSprites; i++){
                    for(int j = 0; j<NumRenderSprites; j++){
                            int I = RenderArray[i]->getRenderPos();
                            int J = RenderArray[j]->getRenderPos();


                        //if((RectsP[j]->getPosY())<(RectsP[j]->getPosY())){
                    if(J>I){
                        RenderSprite* temp = RenderArray[j];
                        RenderArray[j] = RenderArray[i];
                        RenderArray[i] = temp;
                        //std::cout<<"must switch";
                        }

                    }
                }

                for(int i = 0; i<NumRenderSprites ; i ++){
                RenderArray[i]->SetCam(camera.x, camera.y);
                RenderArray[i]->render();

                }
                //enemy1.render();
                //Box1.render();

                        if(debug){

                gFont = TTF_OpenFont( "./lazy.ttf", 20 );
                gTextTexture.loadFromRenderedText(jill.getPosXString()+" "+jill.getPosYString()+" "+jill.getVertString(), TextColor);
                gTextTexture.render( DEBUG_X, DEBUG_Y,1 );

                        }
                        else{
                            gFont = NULL;
                        }

                if(renderflag){

                        //              if(blinker){
                 SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
                  SDL_Rect fillRect = { 500, 100, 50, 50};
                  SDL_RenderFillRect( gRenderer, &fillRect );
                        //                }


                }
                if(renderflag2){

                        //              if(blinker){
                 SDL_SetRenderDrawColor( gRenderer, 0x2a, 0xFF, 0x00, 0x67 );
                  SDL_Rect fillRect = { 600, 100, 50, 50};
                  SDL_RenderFillRect( gRenderer, &fillRect );
                        //                }


                }
                if(renderflag3){

                        //              if(blinker){
                 SDL_SetRenderDrawColor( gRenderer, 0x2a, 0xFF, 0xFF, 0x67 );
                  SDL_Rect fillRect = { 400, 100, 50, 50};
                  SDL_RenderFillRect( gRenderer, &fillRect );
                        //                }


                }
                if(LineRenderFlag){


                // SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );

                //LineOne.RenderLine(camera);

                for(int i = 0; i<CollisionVector.size();i++){
                    CollisionVector.at(i).RenderLine(camera);
                    }
                }

				//Update screen
				SDL_RenderPresent( gRenderer );




			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
