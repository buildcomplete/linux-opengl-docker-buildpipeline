#include "Engine_NetworkDrawingManager.h"
#include <cmath>
#include <raymath.h>
#include "IG_types.h"


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


// END NETWORK DRAWING HELPER METHODS

void Engine_NetworkDrawingManager::Draw(Vector2 worldPos, bool drawMode, float pixPr_cm)
{
     // Network will be arranged on grid borders
    // If there are 0 or 1 neighbours, draw a circle
    // Then draw a line from center of this to each neighbour
    // But this is only the drawing function, so here we only draw connection to other network, and it is not possible to create illigal connections
    if (drawMode)
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

void Engine_NetworkDrawingManager::StartNewNetwork()
{
    // when starting network mode, reset current drawing state (or connect to existing network later on...)
    drawnNetwork.clear();
}

void Engine_NetworkDrawingManager::CompleteDrawing()
{
    // Copy to list of actual networks
    if (drawnNetwork.size() > 1)
    {
        networks.push_back(std::vector<Vector2>(drawnNetwork));
    }
    drawnNetwork.clear();
}

void Engine_NetworkDrawingManager::AddAnchorPoint(Vector2 worldPos, float pixPr_cm)
{
    // if starting a new network, update networkValidToHelper
    // if not starting a new it should be valid from mouse moving
    if (drawnNetwork.size() == 0 )
    {
        networkValidToHelper = QuantifiedNetworkPos(
            worldPos,
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
