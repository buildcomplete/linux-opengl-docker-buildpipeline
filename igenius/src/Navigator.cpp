#include "Navigator.h"
#include "IG_types.h"
#include <iostream>
#include <vector>
#include "Canvas.h"

Navigator::Navigator()
{
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    gameMousePos = {(float)GetScreenWidth() / 2,(float)GetScreenHeight() / 2};
    pixPr_cm = 10;
    DisableCursor();
}

NavigationContext Navigator::HandleEvents(StateContext& sc, float pixPr_cm_)
{
    pixPr_cm = pixPr_cm_;
    if ( sc.DidEnterState(IG_MOUSE_SCREEN_DRAGGING))
    {
        dragAcceleration.x = dragAcceleration.y = 0;
    }
    
    Vector2 md = GetMouseDelta();
    if (sc.IsInState(IG_MOUSE_SCREEN_DRAGGING))
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

    Vector2 mwp = GetScreenToWorld2D(gameMousePos, camera);
    context = { 
        gameMousePos, // mousePosScreenPixel
        mwp, //mousePosWorldPixels
        {mwp.x/pixPr_cm,mwp.y/pixPr_cm},  //mousePosWorldCm,
        { (int)(mwp.x / pixPr_cm), (int)(mwp.y / pixPr_cm)} // mousePosWorldGrid
    };
    return context;
}

// Should be called while drawing in camera mode
void Navigator::DrawCursorWorldGuide(const  StateContext& sc, const Canvas& canvas)
{
    // Assuming drawing in camera mode.
    // draw circle close to where mouse is
    Vector2 worldPos = context.mousePosWorldPixels;
    CellPosition cPos =  context.mousePosWorldGrid;
    Vector2 quantifiedCenter =  { cPos.x * pixPr_cm, cPos.y * pixPr_cm };

    
    if (sc.IsInState(IG_MOUSE_SELECTING))
    {
        for (int r = -3;r<4;++r) 
        {
            for (int c = -3;c<4;++c) 
            {

                // Determine color depending in wheter or not we are overlapping with component or network
                Color circleColor = (canvas.GetCellInfo(cPos.x + c, cPos.y + r).componentId == 0) ? WHITE : RED; 
                Vector2 indicatorPos = {
                    quantifiedCenter.x + c * pixPr_cm,
                    quantifiedCenter.y + r * pixPr_cm };

                
                // the quantified center is the upper left vertic, we need to calculate distance as we are in the center of the vertex.
                float alpha = Clamp(
                    1.0f - Vector2Distance(Vector2AddValue(indicatorPos, 0.5f * pixPr_cm), worldPos) / (2.2f * pixPr_cm), 
                    0.0f, 
                    1.0f);
                if (alpha > 0.01)
                {
                    float marginF = Clamp(
                        1.0f - Vector2Distance(Vector2AddValue(indicatorPos, 0.5f * pixPr_cm), worldPos) / (2.5f * pixPr_cm), 
                        0.2f, 
                        1.0f);
                    int margin = marginF * pixPr_cm/5;

                    circleColor.a = (int)floor(255.0f * alpha);
                    DrawRectangleLines( (int)indicatorPos.x+margin, (int)indicatorPos.y+margin, (int)pixPr_cm-2*margin, (int)pixPr_cm-2*margin, circleColor );
                }
            }
        }
    }
}

// Should be called while drawing in fixed screen coordinates (Outhside camera mode)
void Navigator::DrawCursorScreenGuide(const StateContext& sc)
{
    // Draw cross where we are 
    if (sc.IsInState(IG_MOUSE_SELECTING))
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

    if (sc.IsInState(IG_MOUSE_SCREEN_DRAGGING))
    {
        DrawLine(
            gameMousePos.x - dragAcceleration.x * 1.5, gameMousePos.y - dragAcceleration.y * 1.5,
            gameMousePos.x - dragAcceleration.x * 5, gameMousePos.y - dragAcceleration.y * 5, 
            NETGREEN);
    }

    if (sc.IsInState(IG_MOUSE_MODE_NETWORK))
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
