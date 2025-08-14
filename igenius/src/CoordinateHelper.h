#ifndef COORDINATEHELPER_H
#define COORDINATEHELPER_H

#include "raylib.h"
#include "raymath.h"

/**
 * Provides utility functions for converting between screen coordinates 
 * and grid coordinates based on physical screen dimensions.
 */
class CoordinateHelper
{
public:
    float pixPr_cm = 1;
    CoordinateHelper();
    float CmToPixel(float cm);
};

#endif