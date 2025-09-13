#ifndef NAVIGATION_CONTEXT_H
#define NAVIGATION_CONTEXT_H

#include "raymath.h"
#include "../IG_types.h"

struct NavigationContext
{
    Vector2 mousePosScreenPixel;
    Vector2 mousePosWorldPixels;
    Vector2 mousePosWorldCm;
    CellPosition mousePosWorldGrid;
    Camera2D camera;
    float pixPr_cm;
    
    float CmToPixel(float cm) const {return cm*pixPr_cm;} 
    CellPosition ScreenToCellIndex(const Vector2& screenPos) const {
        Vector2 worldPos = GetScreenToWorld2D(screenPos, camera);
        return { (int)(worldPos.x / pixPr_cm), (int)(worldPos.y / pixPr_cm) };
    }
};

#endif