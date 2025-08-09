#ifndef COORDINATEHELPER_H
#define COORDINATEHELPER_H

#include "raylib.h"
#include "raymath.h"


class CoordinateHelper 
{
    public:
        float pixPr_cm = 1;
        CoordinateHelper();
        float CmToPixel(float cm);
        void DrawHelpers(Vector2 gameMousePos, Camera2D worldPos);
};

#endif