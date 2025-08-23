#include "Engine_NetworkDrawingManager.h"
#include <cmath>
#include <raymath.h>
#include "IG_types.h"
#include <iostream>

// END NETWORK DRAWING HELPER METHODS

void Engine_NetworkDrawingManager::Draw(const NavigationContext& navCtx, const RenderContext& rndrCtx, const StateContext& stateCtx)
{
    auto pixPr_cm = rndrCtx.pixPr_cm;
    // Network will be arranged on grid borders
    // If there are 0 or 1 neighbours, draw a circle
    // Then draw a line from center of this to each neighbour
    // But this is only the drawing function, so here we only draw connection to other network, and it is not possible to create illigal connections
    if (stateCtx.IsInState(MOUSE_MODE_FLAGS::IG_MOUSE_MODE_NETWORK))
    {
        Color circleColor = ELECTRIC_BLUE;
        circleColor.a = 155;
        DrawCircle(navCtx.mousePosWorldGrid.x * pixPr_cm + pixPr_cm / 2.0f, navCtx.mousePosWorldGrid.y * pixPr_cm+ pixPr_cm / 2.0f, pixPr_cm / 6.0f, circleColor);

        if (drawnNetwork.size() != 0)
        {
            CellPosition fromC = networkDrawPos[networkDrawPosIdx];

            Vector2 to = {(float)navCtx.mousePosWorldGrid.x, (float)navCtx.mousePosWorldGrid.y};
            Vector2 from = {(float)fromC.x, (float)fromC.y};
            // Only allow 0,45,90,.. degrees etc.
            Vector2 distance = Vector2Subtract(from, to);
            bool horzOrVert = abs(distance.x) < 0.1 || abs(distance.y) < 0.1;
            bool diagonal = abs(abs(distance.x) - abs(distance.y)) < 0.1 || abs(abs(distance.y) - abs(distance.x)) < 0.1;
            bool distOk = Vector2DistanceSqr(from, to) > 0.1;

            if (distOk && (horzOrVert || diagonal))
            {
                // this should be moved to update state
                networkValidToHelper = navCtx.mousePosWorldGrid;
                anyNewvalidPointInNetwork = true;
            }
            if (distOk)
            {
                DrawLine(from.x * pixPr_cm + pixPr_cm / 2.0f, from.y * pixPr_cm + pixPr_cm / 2.0f, networkValidToHelper.x * pixPr_cm + pixPr_cm / 2.0f, networkValidToHelper.y * pixPr_cm + pixPr_cm / 2.0f, NETGREEN);
            }
            else
            {
                anyNewvalidPointInNetwork = false;
            }
        }
    }
    if (drawnNetwork.size() > 0)
    {
        Canvas::DrawNetworkSegment(drawnNetwork, stateCtx, pixPr_cm);
    }
}

void Engine_NetworkDrawingManager::StartDrawing()
{
    // when starting network mode, reset current drawing state (or connect to existing network later on...)
    drawnNetwork.clear();
}

void Engine_NetworkDrawingManager::CompleteDrawing(Canvas &canvas)
{
    // Copy to list of actual networks
    if (drawnNetwork.size() > 1)
    {
        canvas.AddNetworkSegment(drawnNetwork, 0);
    }
    drawnNetwork.clear();
}

void Engine_NetworkDrawingManager::AddAnchorPoint(const NavigationContext &frameCoord, float pixPr_cm)
{
    // if starting a new network, update networkValidToHelper
    // if not starting a new it should be valid from mouse moving
    if (drawnNetwork.size() == 0)
    {
        networkValidToHelper = frameCoord.mousePosWorldGrid;
        anyNewvalidPointInNetwork = true;
    }
    if (anyNewvalidPointInNetwork)
    {
        drawnNetwork.push_back(networkValidToHelper);
        networkDrawPosIdx = (networkDrawPosIdx + 1) % 2;
        networkDrawPos[networkDrawPosIdx] = networkValidToHelper;
        anyNewvalidPointInNetwork = false;
    }
}

