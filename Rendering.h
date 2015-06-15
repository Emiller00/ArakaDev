#pragma once
#ifndef RENDERING_H
#define RENDERING_H
#include <string>
#include <SDL_hints.h>
#include <SDL.h>
#include <vector>
#include "Collision.h"
#include "Constants.h"
#include <SDL_ttf.h>

class TTF_FONT;
//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile(SDL_Renderer* gRenderer, std::string path);

        //Creates image from font string
        bool loadFromRenderedText(SDL_Renderer* gRenderer, TTF_Font* gFont, std::string textureText, SDL_Color textColor);

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        //Renders texture at given point
        void render(SDL_Renderer* gRenderer, int x, int y, float scaler, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

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
        OcclusionTile(SDL_Rect inRect, LTexture* inTecture, std::vector<CollisionLine> &CollisionlineVector);

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
        void render(SDL_Renderer* gRenderer, float scaler, SDL_Rect* clip = NULL,SDL_Rect* InputScreen = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    private:
        SDL_Rect BGRect;
    };

SDL_Rect BoxRectToSDL_Rect(BoxRect InRect);

#endif
