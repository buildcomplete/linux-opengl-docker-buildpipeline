#include "Navigator.h"
#include <iostream>

Navigator::Navigator()
{
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void Navigator::HandleInput()
{
    // Camera rotation controls
    if (IsKeyDown(KEY_A)) camera.rotation--;
    else if (IsKeyDown(KEY_S)) camera.rotation++;

    isDragging = IsMouseButtonDown(1) || IsKeyDown(KEY_SPACE);
    if (isDragging != wasDraggin)
    {
        wasDraggin=isDragging;
        dragAcceleration.x = dragAcceleration.y = 0;
        if (isDragging)
        {
            DisableCursor();
        }
        else
        {
            EnableCursor();
            SetMousePosition(gameMousePos.x, gameMousePos.y);
        }
    }

    if (isDragging)
    {
        Vector2 md = GetMouseDelta();
        dragAcceleration.x -= md.x * 0.1f;
        dragAcceleration.y -= md.y * 0.1f;
        camera.offset.x += dragAcceleration.x;
        camera.offset.y += dragAcceleration.y;
    }
    else
    {
        gameMousePos = GetMousePosition();
    }
}