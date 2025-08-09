#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "raylib.h"
#include "raymath.h"

class Navigator {
	bool isDragging = false;
	bool wasDraggin = false;
	Vector2 dragAcceleration = {0,0};
	
    public:

        Camera2D camera = { 0 };
        Vector2 gameMousePos = {0,0}; // Hack to store mouse pos between enabling end disabling mouse drag
        Navigator();
        
        void HandleInput();
};

#endif