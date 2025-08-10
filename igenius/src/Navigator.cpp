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
    MOUSE_MODE_FLAGS targetState = IG_MOUSE_ZERO;
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_SPACE)) ? IG_MOUSE_SCREEN_DRAGGING : IG_MOUSE_ZERO));
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((IsKeyDown(KEY_LEFT_SHIFT)) ? IG_MOUSE_MODE_NETWORK : IG_MOUSE_ZERO));
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((IG_MOUSE_MODE_NETWORK & IG_MOUSE_MODE_NETWORK && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? IG_MOUSE_DRAW_NETWORK : IG_MOUSE_ZERO));
    
    // Set MOUSE SELECTING if we are not positioning components or in network mode
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((targetState & (IG_MOUSE_MODE_NETWORK | IG_MOUSE_POSITION_COMPONENT ) ) ? IG_MOUSE_ZERO : IG_MOUSE_SELECTING));
    MOUSE_MODE_FLAGS flippedStates = (MOUSE_MODE_FLAGS)(targetState & ~flags);
    
    if ( IG_MOUSE_SCREEN_DRAGGING & flippedStates)
    {
        dragAcceleration.x = dragAcceleration.y = 0;
    }
    
    Vector2 md = GetMouseDelta();
    if (IG_MOUSE_SCREEN_DRAGGING & targetState)
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
    flags = (MOUSE_MODE_FLAGS)targetState;
}

void Navigator::DrawCursorWorldGuide(float pixPr_cm)
{
    // Assuming drawing in camera mode.
    // draw circle close to where mouse is
    Vector2 worldPos = GetScreenToWorld2D(gameMousePos, camera);

    Vector2 quantifiedCenter =  { 
        (float)round(worldPos.x / pixPr_cm) * pixPr_cm,
        (float)round(worldPos.y / pixPr_cm) * pixPr_cm
    };


    if (IG_MOUSE_SELECTING & flags)
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

    // Network will be arranged on grid borders
    // If there are 0 or 1 neighbours, draw a circle
    // Then draw a line from center of this to each neighbour
    // But this is only the drawing function, so here we only draw connection to other network, and it is not possible to create illigal connections
    if (IG_MOUSE_MODE_NETWORK & flags)
    {        
         Vector2 indicatorPos = {
            quantifiedCenter.x ,
            quantifiedCenter.y };
        
        Vector2 delta = Vector2Subtract(worldPos, indicatorPos);
        float dxs = delta.x <= 0 ? -.5f : .5f;
        float dys = delta.y <= 0 ? -.5f : .5f;
        
        Color circleColor = ELECTRIC_BLUE;
        circleColor.a = 155;
        DrawCircle( indicatorPos.x + pixPr_cm * dxs, indicatorPos.y + pixPr_cm * dys, pixPr_cm / 6.0f, circleColor );


    }
}
void Navigator::DrawCursorScreenGuide()
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

    if (IG_MOUSE_MODE_NETWORK & flags)
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
