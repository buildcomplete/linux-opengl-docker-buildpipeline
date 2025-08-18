#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "raylib.h"
#include "raymath.h"
#include "Engine_StateManager.h"
#include "Canvas.h"

/**
 * Handles mouse interactions, including dragging and cursor customization, 
 * while querying the Canvas for item types to enhance user experience.
 */
class Navigator {
	
    public:

        Navigator();
        IG_MouseCoordinates SyncWithState(Engine_StateManager &eMan, float pixPr_cm_);
        void DrawCursorWorldGuide(const IG_MouseCoordinates &frameCoords, const Engine_StateManager &eMan, const Canvas &canvas);
        void DrawCursorScreenGuide(const Engine_StateManager& eMan);
        Camera2D camera = { 0 };

    private:
        float pixPr_cm;
        Vector2 dragAcceleration = {0,0};
        Vector2 gameMousePos = {0,0}; // Store mouse pos, to enable drag and consistent behavior, we use internal logic instead of Raylib cursor to draw and position cursor
    
};

#endif

