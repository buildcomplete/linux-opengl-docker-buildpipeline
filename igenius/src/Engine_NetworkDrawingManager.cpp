#include "Engine_NetworkDrawingManager.h"
#include <cmath>
#include <raymath.h>
#include "IG_types.h"
#include <rlgl.h>


// BEGIN NETWORK DRAWING HELPERS, SHOULD BE MOVED SOMEWHERE ELSE
CellPosition QuantifiedNetworkPos(Vector2 worldPos, float pixPr_cm)
{
    Vector2 quantifiedCenter =  { 
        (float)round(worldPos.x / pixPr_cm) * pixPr_cm,
        (float)round(worldPos.y / pixPr_cm) * pixPr_cm
    };

    Vector2 delta = Vector2Subtract(worldPos, quantifiedCenter);
    float dxs = delta.x <= 0 ? -.5f : .5f;
    float dys = delta.y <= 0 ? -.5f : .5f;
    return {(int)(quantifiedCenter.x +dxs * pixPr_cm), (int)(quantifiedCenter.y +dys * pixPr_cm) };
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
        CellPosition netPos = QuantifiedNetworkPos(worldPos, pixPr_cm);

        Color circleColor = ELECTRIC_BLUE;
        circleColor.a = 155;
        DrawCircle( netPos.x,netPos.y, pixPr_cm / 6.0f, circleColor );

        if (drawnNetwork.size() != 0 )
        {
            CellPosition toC =  QuantifiedNetworkPos(worldPos, pixPr_cm);
            CellPosition fromC = networkDrawPos[networkDrawPosIdx];
            
            Vector2 to = {(float)toC.x, (float)toC.y};
            Vector2 from =  {(float)fromC.x, (float)fromC.y};
            // Only allow 0,45,90,.. degrees etc.
            Vector2 distance = Vector2Subtract(from,to);
            bool horzOrVert = abs(distance.x) < 0.1 || abs(distance.y) < 0.1;
            bool diagonal = abs(abs(distance.x) - abs(distance.y)) < 0.1 || abs(abs(distance.y) - abs(distance.x)) < 0.1;
            bool distOk = Vector2DistanceSqr(from, to) > 1;

            if ( distOk && (horzOrVert || diagonal))
            {
                // this should be moved to update state
                networkValidToHelper = toC;
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
        DrawNetworkSegment(drawnNetwork, drawMode, pixPr_cm);
    }

    for (int i=0;i<networks.size();  ++i)
    {
        DrawNetworkSegment(networks[i], drawMode, pixPr_cm);
    }
}

void Engine_NetworkDrawingManager::StartNewNetwork()
{
    // when starting network mode, reset current drawing state (or connect to existing network later on...)
    drawnNetwork.clear();
}

void Engine_NetworkDrawingManager::CompleteDrawing(Canvas& canvas)
{
    // Copy to list of actual networks
    if (drawnNetwork.size() > 1)
    {
        //canvas.AddNetwork(drawnNetwork);
        networks.push_back(std::vector<CellPosition>(drawnNetwork));
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

// Small copy paste from raylib to support drawing integer line segments
// Draw lines sequuence (using gl lines)
void DrawLineStripCellPos(const CellPosition *points, int pointCount, Color color)
{
    if (pointCount < 2) return; // Security check

    rlBegin(RL_LINES);
        rlColor4ub(color.r, color.g, color.b, color.a);

        for (int i = 0; i < pointCount - 1; i++)
        {
            rlVertex2i(points[i].x, points[i].y);
            rlVertex2i(points[i + 1].x, points[i + 1].y);
        }
    rlEnd();
}

void Engine_NetworkDrawingManager::DrawNetworkSegment(std::vector<CellPosition>& network, bool drawNodes, float pixPr_cm)
{
    DrawLineStripCellPos(&(network[0]), network.size(), NETGREEN );
    if (drawNodes)
    {
        for (int i=0;i<network.size(); ++i)
        {
            DrawCircleLines(network[i].x, network[i].y, pixPr_cm / 10.0f, NETGREEN);
        }
    }
}


