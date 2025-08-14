#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "raylib.h"
#include "raymath.h"
#include "Engine_StateManager.h"

/**
 * Handles mouse interactions, including dragging and cursor customization, 
 * while querying the Canvas for item types to enhance user experience.
 */
class Navigator {
	Vector2 dragAcceleration = {0,0};
    
    public:

        Camera2D camera = { 0 };
        Vector2 gameMousePos = {0,0}; // Hack to store mouse pos between enabling end disabling mouse drag
        Navigator();
        
        void SyncWithState(Engine_StateManager& eMan, float pixPr_cm);
        void DrawCursorWorldGuide(Engine_StateManager& eMan, float pixPr_cm);
        void DrawCursorScreenGuide(Engine_StateManager& eMan);
        Vector2 GetMousePosWorld();

};

#endif

