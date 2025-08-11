#include "Navigator.h"
#include "ig_types.h"
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

inline bool did_enter_state(MOUSE_MODE_FLAGS testState, MOUSE_MODE_FLAGS newState, MOUSE_MODE_FLAGS flippedStates)
{
    return testState & newState & flippedStates;
}
inline bool did_exit_state(MOUSE_MODE_FLAGS testState, MOUSE_MODE_FLAGS newState, MOUSE_MODE_FLAGS flippedStates)
{
    return testState & ~newState & flippedStates;
}


// BEGIN NETWORK DRAWING HELERS, SHOULD BE MOVED SOMEWHERE ELSE
Vector2 QuantifiedNetworkPos(Vector2 worldPos, float pixPr_cm)
{
    Vector2 quantifiedCenter =  { 
        (float)round(worldPos.x / pixPr_cm) * pixPr_cm,
        (float)round(worldPos.y / pixPr_cm) * pixPr_cm
    };

    Vector2 delta = Vector2Subtract(worldPos, quantifiedCenter);
    float dxs = delta.x <= 0 ? -.5f : .5f;
    float dys = delta.y <= 0 ? -.5f : .5f;
    return Vector2Add(quantifiedCenter, {dxs * pixPr_cm, dys * pixPr_cm});
}

bool startNewDrawing = true;
int networkDrawPosIdx = 1;
Vector2 networkDrawPos[2] = {{0,0},{0,0}};
Vector2 networkValidToHelper = {0,0};
std::vector<Vector2> drawnNetwork= std::vector<Vector2>(5);
// END NEWORK DRAWING HELPER METHODS

void Navigator::HandleInput(float pixPr_cm)
{
    MOUSE_MODE_FLAGS targetState = IG_MOUSE_ZERO;
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_SPACE)) ? IG_MOUSE_SCREEN_DRAGGING : IG_MOUSE_ZERO));
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((IsKeyDown(KEY_LEFT_SHIFT)) ? IG_MOUSE_MODE_NETWORK : IG_MOUSE_ZERO));
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((IG_MOUSE_MODE_NETWORK & IG_MOUSE_MODE_NETWORK && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? IG_MOUSE_DRAW_NETWORK : IG_MOUSE_ZERO));
    
    // Set MOUSE SELECTING if we are not positioning components or in network mode
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((targetState & (IG_MOUSE_MODE_NETWORK | IG_MOUSE_POSITION_COMPONENT ) ) ? IG_MOUSE_ZERO : IG_MOUSE_SELECTING));
    MOUSE_MODE_FLAGS flippedStates = (MOUSE_MODE_FLAGS)(targetState & ~flags);

    if (did_enter_state(IG_MOUSE_MODE_NETWORK,targetState, flippedStates ))
    {
        // when starting network mode, reset current drawing state (or connect to existing network later on...)
        startNewDrawing = true;
        drawnNetwork.clear();
    }

    // when starting network draw
    if (did_enter_state(IG_MOUSE_DRAW_NETWORK,targetState, flippedStates ))
    {
        if (drawnNetwork.size() != 0 )
            drawnNetwork.push_back(networkValidToHelper);

        networkDrawPosIdx = (networkDrawPosIdx + 1) % 2;
        networkValidToHelper = QuantifiedNetworkPos(
            GetScreenToWorld2D(gameMousePos, camera),
            pixPr_cm);
        networkDrawPos[networkDrawPosIdx] = networkValidToHelper;
        if (!startNewDrawing)
            drawnNetwork.push_back(networkValidToHelper);

        startNewDrawing=false;
        
        if (drawnNetwork.size() == 0 )
            drawnNetwork.push_back(networkValidToHelper);
    }

    
    
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
        Vector2 netPos = QuantifiedNetworkPos(worldPos, pixPr_cm);

        Color circleColor = ELECTRIC_BLUE;
        circleColor.a = 155;
        DrawCircle( netPos.x,netPos.y, pixPr_cm / 6.0f, circleColor );

        if (!startNewDrawing)
        {
            Vector2 to = QuantifiedNetworkPos(worldPos, pixPr_cm);
            Vector2 from = networkDrawPos[networkDrawPosIdx];
            // Only allow 0,45,90,.. degrees etc.
            Vector2 distance = Vector2Subtract(from,to);
            bool horzOrVert = abs(distance.x) < 0.1 || abs(distance.y) < 0.1;
            bool diagonal = abs(abs(distance.x) - abs(distance.y)) < 0.1 || abs(abs(distance.y) - abs(distance.x)) < 0.1;
            bool distOk = Vector2DistanceSqr(from, to) > 1;

            if ( distOk && (horzOrVert || diagonal))
            {
                networkValidToHelper = to;
            }
            if (distOk)
            {
                DrawLine(from.x, from.y, networkValidToHelper.x, networkValidToHelper.y, ELECTRIC_BLUE);
            }

            if (drawnNetwork.size() > 1)
            {
                DrawLineStrip(&(drawnNetwork[0]), drawnNetwork.size(), ELECTRIC_BLUE );
            }
        }
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
