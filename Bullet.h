#pragma once
#ifndef BULLET_H
#define BULLET_H
#include <vector>

struct BulletCoord;

class BulletHandler{
    public:
        BulletHandler();

        // Make and destroy functions
        // These will return true if they can succesfully create or destroy a bullet
        bool MakeBullet(float nX, float nY, bool IsFlipped);
        bool DestroyBullet(int index);
    std::vector<BulletCoord> GetBullets();
        BulletCoord GetBullet(int index);
        int NumBullets();
        void BulletX(int index, float deltaX);
        // void BulletY(int index, int deltaY);
    private:

    std::vector<BulletCoord> Bulletarray;
};

#endif
