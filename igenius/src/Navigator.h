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
        NavigationContext HandleEvents(StateContext &sc, float pixPr_cm_);
        void DrawCursorWorldGuide(const StateContext &sc, const Canvas &canvas);
        void DrawCursorScreenGuide(const StateContext &sc);
        Camera2D camera = { 0 };

    private:
        NavigationContext context;
        float pixPr_cm;
        Vector2 dragAcceleration = {0,0};
        Vector2 gameMousePos = {0,0}; // Store mouse pos, to enable drag and consistent behavior, we use internal logic instead of Raylib cursor to draw and position cursor
    
};

#endif

