#include "NetworkManager.h"
#include <cmath>
#include <raymath.h>
#include "IG_types.h"
#include <iostream>


void NetworkManager::HandleEvents(const StateContext& sc)
{
    
}

void NetworkManager::Draw(const RenderContext& rndrCtx, const NavigationContext& navCtx, const StateContext& stateCtx)
{
    auto pixPr_cm = navCtx.pixPr_cm;
    // Network will be arranged on grid centers
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
            CellPosition toC = navCtx.mousePosWorldGrid;
            CellPosition fromC = networkDrawPos[networkDrawPosIdx];

            if (CanAddToNetwork(fromC, toC))
            {
                // this should be moved to update state
                networkValidToHelper = navCtx.mousePosWorldGrid;
                anyNewvalidPointInNetwork = true;
            }
            
            if (anyNewvalidPointInNetwork)
            {
                DrawLine(fromC.x * pixPr_cm + pixPr_cm / 2.0f, fromC.y * pixPr_cm + pixPr_cm / 2.0f, networkValidToHelper.x * pixPr_cm + pixPr_cm / 2.0f, networkValidToHelper.y * pixPr_cm + pixPr_cm / 2.0f, NETGREEN);

                //anyNewvalidPointInNetwork = false;
            }
        }
    }
    if (drawnNetwork.size() > 0)
    {
        Canvas::DrawNetworkSegment(drawnNetwork, stateCtx, pixPr_cm);
    }
}

bool NetworkManager::CanAddToNetwork(CellPosition toC)
{
    return drawnNetwork.size() == 0 || CanAddToNetwork(networkDrawPos[networkDrawPosIdx], toC);
}
bool NetworkManager::CanAddToNetwork(CellPosition fromC, CellPosition toC)
{
    Vector2 to = {(float)toC.x, (float)toC.y};
    Vector2 from = {(float)fromC.x, (float)fromC.y};
    // Only allow 0,45,90,.. degrees etc.
    Vector2 distance = Vector2Subtract(from, to);
    bool horzOrVert = abs(distance.x) < 0.1 || abs(distance.y) < 0.1;
    bool diagonal = abs(abs(distance.x) - abs(distance.y)) < 0.1 || abs(abs(distance.y) - abs(distance.x)) < 0.1;
    bool distOk = Vector2DistanceSqr(from, to) > 0.1; // line non zero length
    return distOk && (horzOrVert || diagonal);
}

void NetworkManager::StartDrawing()
{
    // when starting network mode, reset current drawing state (or connect to existing network later on...)
    drawnNetwork.clear();
}



std::vector<CellPosition> NetworkManager::CompleteDrawing()
{
    auto drawnNetworkCp = std::vector<CellPosition>(drawnNetwork);
    drawnNetwork.clear();
    
    if (drawnNetworkCp.size() < 2)
        drawnNetworkCp.clear();
    return drawnNetworkCp;
    
}

void NetworkManager::AddAnchorPoint(const NavigationContext &frameCoord)
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

// Tryies to add an anchor point, 
// if requested position is blocked or a path cannot be found from previus ancor return false
// if point could be added, returns true
bool NetworkManager::TryAddAnchorPoint(CellPosition anchor)
{

    // Todo, update so a path is found following the horizontal/vertical line rules
    if (!CanAddToNetwork(anchor))
        return false;
    
    drawnNetwork.push_back(anchor);
    return true;
}

