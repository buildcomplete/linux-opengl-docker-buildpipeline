#ifndef COORDINATEHELPER_H
#define COORDINATEHELPER_H

#include "raylib.h"
#include "raymath.h"


class CoordinateHelper 
{
    public:
        float res = 1;
        CoordinateHelper();
        void Draw(Vector2 gameMousePos, Camera2D worldPos);

};

#endif