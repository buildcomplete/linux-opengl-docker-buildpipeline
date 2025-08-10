#include "Navigator.h"
#include "ig_types.h"
#include <iostream>

Navigator::Navigator()
{
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    gameMousePos = {(float)GetScreenWidth() / 2,(float)GetScreenHeight() / 2};
    DisableCursor();
}

void Navigator::HandleInput()
{
    // Camera rotation controls
    if (IsKeyDown(KEY_A)) camera.rotation--;
    else if (IsKeyDown(KEY_S)) camera.rotation++;

    bool isDragging = IsMouseButtonDown(1) || IsKeyDown(KEY_SPACE);
    bool isSettingNetwork = IsKeyDown(KEY_LEFT_SHIFT);

    if (isSettingNetwork)
    {
        flags = (MOUSE_MODE_FLAGS)(flags & ~IG_MOUSE_SELECTING | IG_MOUSE_DRAW_NETWORK);
    }
    else
    {
        flags = (MOUSE_MODE_FLAGS)(flags & ~IG_MOUSE_DRAW_NETWORK | IG_MOUSE_SELECTING );
    }
    
    
    if (isDragging != wasDraggin)
    {
        wasDraggin = isDragging;

         if (isDragging)
            flags = (MOUSE_MODE_FLAGS)(flags | IG_MOUSE_SCREEN_DRAGGING);
        else
            flags = (MOUSE_MODE_FLAGS)(flags & ~IG_MOUSE_SCREEN_DRAGGING);
        
        dragAcceleration.x = dragAcceleration.y = 0;
    }
    
    Vector2 md = GetMouseDelta();
    if (isDragging)
    {
        dragAcceleration.x -= md.x * 0.1f;
        dragAcceleration.y -= md.y * 0.1f;
        camera.offset.x += dragAcceleration.x;
        camera.offset.y += dragAcceleration.y;

        // Limit drag acceleration speed
        float dragSQ = Vector2LengthSqr(dragAcceleration);
        const float maxDragSQ = 200.0f;
        if (dragSQ > maxDragSQ)
        {
            const float maxDrag = sqrt(maxDragSQ);
            float drag = sqrt(dragSQ);
            dragAcceleration = Vector2Scale(dragAcceleration, maxDrag/drag);
        }
    }
    else
    {
        gameMousePos.x  += md.x;
        gameMousePos.y  += md.y;
    }
}

void Navigator::Draw()
{
    // Draw cross where we are 
    if (IG_MOUSE_SELECTING & flags )
    {
        DrawLine(
            gameMousePos.x - 10, gameMousePos.y ,
            gameMousePos.x + 10, gameMousePos.y, 
            NETGREEN);
        DrawLine(
            gameMousePos.x , gameMousePos.y - 10,
            gameMousePos.x , gameMousePos.y + 10, 
            NETGREEN);
    }

    if (IG_MOUSE_SCREEN_DRAGGING & flags)
    {
        DrawLine(
            gameMousePos.x - dragAcceleration.x * 1.5, gameMousePos.y - dragAcceleration.y * 1.5,
            gameMousePos.x - dragAcceleration.x * 5, gameMousePos.y - dragAcceleration.y * 5, 
            NETGREEN);
    }
}
