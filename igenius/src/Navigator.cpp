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


// BEGIN NETWORK DRAWING HELPERS, SHOULD BE MOVED SOMEWHERE ELSE
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

int networkDrawPosIdx = 1;
Vector2 networkDrawPos[2] = {{0,0},{0,0}};
Vector2 networkValidToHelper = {0,0};
std::vector<Vector2> drawnNetwork= std::vector<Vector2>(5);
bool anyNewvalidPointInNetwork = false;
std::vector<std::vector<Vector2>> networks = std::vector<std::vector<Vector2>>(5);
// END NETWORK DRAWING HELPER METHODS

void Navigator::SyncWithState(Engine_StateManager& eMan, float pixPr_cm)
{
    if (eMan.DidEnterState(IG_MOUSE_MODE_NETWORK))
    {
        // when starting network mode, reset current drawing state (or connect to existing network later on...)
            drawnNetwork.clear();
    }

    if (eMan.DidExitState(IG_MOUSE_MODE_NETWORK))
    {
        // Copy to list of actual networks
        if (drawnNetwork.size() > 1)
        {
            networks.push_back(std::vector<Vector2>(drawnNetwork));
        }
        drawnNetwork.clear();
    }

    // when starting network draw
    if (eMan.DidEnterState(IG_MOUSE_DRAW_NETWORK ))
    {
        // if starting a new network, update networkValidToHelper
        // if not starting a new it should be valid from mouse moving
        if (drawnNetwork.size() == 0 )
        {
            networkValidToHelper = QuantifiedNetworkPos(
                GetScreenToWorld2D(gameMousePos, camera),
                pixPr_cm);
            anyNewvalidPointInNetwork = true;
        }
        if (anyNewvalidPointInNetwork)
        {
            drawnNetwork.push_back(networkValidToHelper);
            networkDrawPosIdx = (networkDrawPosIdx + 1) % 2;
            networkDrawPos[networkDrawPosIdx] = networkValidToHelper;
            anyNewvalidPointInNetwork=false;
        }
    }
    
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


void Navigator::DrawCursorWorldGuide(Engine_StateManager& eMan, float pixPr_cm)
{
    // Assuming drawing in camera mode.
    // draw circle close to where mouse is
    Vector2 worldPos = GetScreenToWorld2D(gameMousePos, camera);

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

    // Network will be arranged on grid borders
    // If there are 0 or 1 neighbours, draw a circle
    // Then draw a line from center of this to each neighbour
    // But this is only the drawing function, so here we only draw connection to other network, and it is not possible to create illigal connections
    if (IG_MOUSE_MODE_NETWORK & eMan.GetFlags())
    {
        Vector2 netPos = QuantifiedNetworkPos(worldPos, pixPr_cm);

        Color circleColor = ELECTRIC_BLUE;
        circleColor.a = 155;
        DrawCircle( netPos.x,netPos.y, pixPr_cm / 6.0f, circleColor );

        if (drawnNetwork.size() != 0 )
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
                anyNewvalidPointInNetwork= true;
            }
            if (distOk)
            {
                DrawLine(from.x, from.y, networkValidToHelper.x, networkValidToHelper.y, NETGREEN);
            }
            else
            {
                anyNewvalidPointInNetwork=false;
            }
        }
    }
    if (drawnNetwork.size() > 0)
    {
        DrawLineStrip(&(drawnNetwork[0]), drawnNetwork.size(), NETGREEN );
    }

    for (int i=0;i<networks.size();  ++i)
    {
        DrawLineStrip(&(networks[i][0]), networks[i].size(), NETGREEN );
    }
    
}
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
