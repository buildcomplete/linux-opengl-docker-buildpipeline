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
       return ScreenToCellIndexStatic(screenPos, camera, pixPr_cm);
    }

    Vector2 ScreenToWPCM(const Vector2& screenPos) const {
        return ScreenToWPCMStatic(screenPos, camera, pixPr_cm);
    }

    static CellPosition ScreenToCellIndexStatic(const Vector2& screenPos, const Camera2D& camera, float pixPr_cm) {
        Vector2 worldPosCM = ScreenToWPCMStatic(screenPos, camera, pixPr_cm);
        return { (int)(worldPosCM.x), (int)(worldPosCM.y) };
    }
    static Vector2 ScreenToWPCMStatic(const Vector2& screenPos, const Camera2D& camera, float pixPr_cm) {
        Vector2 worldPos = GetScreenToWorld2D(screenPos, camera);
        return { (worldPos.x / pixPr_cm), (worldPos.y / pixPr_cm) };
    }
};

#endif