#ifndef NAVIGATION_CONTEXT_H
#define NAVIGATION_CONTEXT_H

#include "raymath.h"
#include "../IG_types.h"

struct NavigationContext
{
    Vector2 mousePosScreenPixel;
    Vector2 mousePosWorldCm;
    CellPosition mousePosWorldGrid;
};

#endif