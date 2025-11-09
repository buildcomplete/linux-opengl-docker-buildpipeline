#pragma once

#include "raylib.h"
#include "raymath.h"
#include "Engine_StateManager.h"
#include "canvas/Canvas.h"

/**
 * Handles mouse interactions, including dragging and cursor customization, 
 * while querying the Canvas for item types to enhance user experience.
 */
class Navigator {
	
    public:

        Navigator();
        NavigationContext HandleEvents(StateContext &sc, float pixPr_cm_);
        void EnforceCameraBounds(float pixPr_cm_);
        void FixCameraBounds(Vector2 &topLeftWC, float pixPr_cm_, Vector2 &bottomRightWC);
        void DrawCursorWorldGuide(const StateContext &sc, const Canvas &canvas) const;
        void DrawCursorScreenGuide(const StateContext &sc) const;
        Camera2D camera = { 0 };

    private:
        NavigationContext context;
        Vector2 dragAcceleration = {0,0};
        Vector2 gameMousePos = {0,0}; // Store mouse pos, to enable drag and consistent behavior, we use internal logic instead of Raylib cursor to draw and position cursor
        Rectangle cameraBoundsWC = {-0x1E,-0x0E, 255, 255}; // World bounds in cm, notice grid relates to upper right corner of cell
    
};
