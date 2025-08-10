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
};

#endif