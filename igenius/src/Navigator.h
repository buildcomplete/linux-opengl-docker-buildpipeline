#ifndef NAVIGATOR_H
#define NAVIGATOR_H

typedef enum
{
    IG_MOUSE_SELECTING 			    = 1 << 0,
    IG_MOUSE_SCREEN_DRAGGING 		= 1 << 1,
    IG_MOUSE_DRAW_NETWORK			= 1 << 2,
    IG_MOUSE_POSITION_COMPONENT 	= 1 << 3,
} MOUSE_MODE_FLAGS;

#include "raylib.h"
#include "raymath.h"

class Navigator {
	bool wasDraggin = false;
	Vector2 dragAcceleration = {0,0};
    MOUSE_MODE_FLAGS flags = IG_MOUSE_SELECTING;
    
    public:

        Camera2D camera = { 0 };
        Vector2 gameMousePos = {0,0}; // Hack to store mouse pos between enabling end disabling mouse drag
        Navigator();
        
        void HandleInput();
        void Draw();
        void UpdateMouseState(MOUSE_MODE_FLAGS &flags);
};

#endif