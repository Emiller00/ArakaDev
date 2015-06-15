#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

struct BulletCoord{
    float mX;
    float mY;
    bool flipped;
};

struct CollisionCoord{
    float mX;
    float mY;
};
struct PatrolPoint{

    PatrolPoint* NextPoint;
    int mX;
    int mY;

};
#endif
