#include "Navigator.h"
#include "IG_types.h"
#include <iostream>
#include <vector>

Navigator::Navigator()
{
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    gameMousePos = {(float)GetScreenWidth() / 2,(float)GetScreenHeight() / 2};
    DisableCursor();
}

void Navigator::SyncWithState(Engine_StateManager& eMan, float pixPr_cm)
{    
    if ( eMan.DidEnterState(IG_MOUSE_SCREEN_DRAGGING))
    {
        dragAcceleration.x = dragAcceleration.y = 0;
    }
    
    Vector2 md = GetMouseDelta();
    if (IG_MOUSE_SCREEN_DRAGGING & eMan.GetFlags())
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
    // If not dragging update mouse positions
    else
    {
        gameMousePos.x  += md.x;
        gameMousePos.y  += md.y;
    }
}

Vector2 Navigator::GetMousePosWorld()
{
    return GetScreenToWorld2D(gameMousePos, camera);
}

// Should be called while drawing in camera mode
void Navigator::DrawCursorWorldGuide(Engine_StateManager& eMan, float pixPr_cm)
{
    // Assuming drawing in camera mode.
    // draw circle close to where mouse is
    Vector2 worldPos = GetMousePosWorld();
    Vector2 quantifiedCenter =  { 
        (float)round(worldPos.x / pixPr_cm) * pixPr_cm,
        (float)round(worldPos.y / pixPr_cm) * pixPr_cm
    };

    if (IG_MOUSE_SELECTING & eMan.GetFlags())
    {
        for (int r = -2;r<3;++r) 
        {
            for (int c = -2;c<3;++c) 
            {
                Vector2 indicatorPos = {
                    quantifiedCenter.x + c*pixPr_cm,
                    quantifiedCenter.y + r*pixPr_cm };

                Color circleColor = WHITE;
                float alpha = Clamp(
                    1.0f - Vector2Distance(worldPos, indicatorPos) / (2.5f * pixPr_cm), 
                    0.0f, 
                    1.0f);
                
                circleColor.a = (int)floor(255.0f * alpha);
    
                if (alpha !=0)
                    DrawCircleLines( indicatorPos.x, indicatorPos.y, pixPr_cm / 6.0f, circleColor );
            }
        }
    }
}

// Should be called while drawing in fixed screen coordinates (Outhside camera mode)
void Navigator::DrawCursorScreenGuide(Engine_StateManager& eMan)
{
    // Draw cross where we are 
    if (IG_MOUSE_SELECTING & eMan.GetFlags() )
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

    if (IG_MOUSE_SCREEN_DRAGGING & eMan.GetFlags())
    {
        DrawLine(
            gameMousePos.x - dragAcceleration.x * 1.5, gameMousePos.y - dragAcceleration.y * 1.5,
            gameMousePos.x - dragAcceleration.x * 5, gameMousePos.y - dragAcceleration.y * 5, 
            NETGREEN);
    }

    if (IG_MOUSE_MODE_NETWORK & eMan.GetFlags())
    {
        DrawLine(
            gameMousePos.x - 10, gameMousePos.y ,
            gameMousePos.x + 10, gameMousePos.y, 
            ELECTRIC_BLUE);
        DrawLine(
            gameMousePos.x , gameMousePos.y - 10,
            gameMousePos.x , gameMousePos.y + 10, 
            ELECTRIC_BLUE);
    }
}
