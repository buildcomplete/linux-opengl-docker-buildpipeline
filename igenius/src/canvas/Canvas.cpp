#include "Canvas.h"
#include <iostream>
#include <rlgl.h>
#include "menus/UI_RadialMenuIMGIMGOperation.h"

// Small copy paste from raylib to support drawing integer line segments in cm space
// Draw lines sequence (using gl lines)
static void DrawLineStripCellPos(const CellPosition *points, int pointCount, Color color, float pixPr_cm)
{
    if (pointCount < 2)
        return; // Security check

    rlBegin(RL_LINES);
    rlColor4ub(color.r, color.g, color.b, color.a);

    for (int i = 0; i < pointCount - 1; i++)
    {
        rlVertex2f(points[i].x * pixPr_cm + pixPr_cm / 2.0f, points[i].y * pixPr_cm + pixPr_cm / 2.0f);
        rlVertex2f(points[i + 1].x * pixPr_cm + pixPr_cm / 2.0f, points[i + 1].y * pixPr_cm + pixPr_cm / 2.0f);
    }
    rlEnd();
}

Canvas::Canvas()
{
    unsigned char i = 0;
    do
    {
        i++;
        avaliableComponentKeys.insert(i);
        avaliableNetworkKeys.insert(i);
        /* code */
    } while (i != 255);
}

bool Canvas::AddComponent(const ComponentBluePrint &bluePrint, unsigned char cellAnchorX, unsigned char cellAnchorY)
{
    if (avaliableComponentKeys.empty())
        return false;

    // Add check if position is valid
    if (!IsGridFree(bluePrint, cellAnchorX, cellAnchorY))
        return false;

    std::uint8_t insertId = *(avaliableComponentKeys.begin());

    // Update lookup table
    SetGridCellValues(bluePrint, cellAnchorX, cellAnchorY, insertId);

    // Update constraint map
    SetConstraints(bluePrint, cellAnchorX, cellAnchorY, NetworkConstraintFlags::FLAG_NCONSTRAINT_ALLDIR);


    // components[insertId] = new UI_Component({insertId, {cellAnchorX, cellAnchorY}, bluePrint});
    components[insertId] = ComponentFactory::CreateUI_Component(bluePrint, insertId, {cellAnchorX, cellAnchorY});

    avaliableComponentKeys.erase(insertId);
    inUseComponentKeys.insert(insertId);
    return true;
}

bool Canvas::ReleaseComponent(std::uint8_t id)
{
    if(HaveComponent(id))
    {
        SetGridCellValues(components[id]->bluePrint, components[id]->anchor.x, components[id]->anchor.y, 0);
        // Todo disconnect the component from any attached networks
        components[id] = nullptr;
        inUseComponentKeys.erase(id);
        avaliableComponentKeys.insert(id);
        return true;
    }
    return false;
}

void canvasTestClickHandler(int it, int type)
{
    std::cout << "> void canvasTestClickHandler(int " << it << "=" << (char)type << ")" << std::endl;
}

void Canvas::Draw(const RenderContext &rc, const NavigationContext &navC, const StateContext &sc) const
{
    DrawBounds(navC, 32, 32);

    for (const auto &i : inUseComponentKeys)
    {
        components[i]->Draw(rc, navC);
    }

    for (int i = 0; i < networks.size(); ++i)
    {
        Canvas::DrawNetworkSegment(networks[i], sc.IsInState(IG_INPUT_MODE_DRAW_NETWORK), navC.pixPr_cm, NETGREEN);
    }

    static auto rMenu = UI_RadialMenuIMGIMGOperation(canvasTestClickHandler);
    if (IsKeyPressed(KEY_H))
    {
        if (rMenu.IsVisible())
            rMenu.Hide();
        else
            rMenu.Show(navC.mousePosWorldCm);
    }

    // HACK TEST OF DRAWING RADIAL MENU
    {
        rMenu.HandleEventsAndTime(sc, navC);
        rMenu.Draw(rc, navC);
    }
}

inline bool IsCellEven(int x, int y) {
    return ((x + y) % 2) == 0;
}

void Canvas::DrawOverlay(const RenderContext &rc, const NavigationContext &navC) const
{
    // Draw grid gutters
    CellPosition UL_CP = navC.ScreenToCellIndex({0, 0});
    CellPosition BR_CP = navC.ScreenToCellIndex({(float)GetScreenWidth(), (float)GetScreenHeight()});
    for (int xi=UL_CP.x-1; xi<=BR_CP.x; ++xi)
    {
        // Get the screen space position for a 2d camera world space position
        Vector2 SP = GetWorldToScreen2D({(float)xi* navC.pixPr_cm, 0}, navC.camera); 
        Color gutterColor = IsCellEven(xi, 0) ? GREEN : DARKGREEN;
        DrawRectangle(SP.x-1, 0, navC.pixPr_cm+1, navC.pixPr_cm * 0.25f, gutterColor);
    }
    for (int yi=UL_CP.y-1; yi<=BR_CP.y; ++yi)
    {
        // Get the screen space position for a 2d camera world space position
        Vector2 SP = GetWorldToScreen2D({0, (float)yi* navC.pixPr_cm}, navC.camera); 
        Color gutterColor = IsCellEven(0, yi) ? GREEN : DARKGREEN;
        DrawRectangle(0, SP.y-1, navC.pixPr_cm * 0.25f, navC.pixPr_cm+1, gutterColor);
    }
}

void Canvas::DrawBounds(const NavigationContext &navC, float limRCM, float limBCM) const
{
    float limRWCP = navC.CmToPixel(limRCM);
    float limBWCP = navC.CmToPixel(limBCM);

    // calculate screen pos of grid bounds
    Vector2 XX0 = GetWorldToScreen2D({0, 0}, navC.camera);                                                                                                // World to screen of world 0,0
    Vector2 XX1 = GetWorldToScreen2D({(float)navC.CmToPixel(limRCM), (float)navC.CmToPixel(limBCM)}, navC.camera);                                        // World to screen of world limit
    Vector2 XXX0 = Vector2Scale(navC.ScreenToWPCMStatic({0, 0}, navC.camera, navC.pixPr_cm), navC.pixPr_cm);                                              // Screen to world of screen 0,0
    Vector2 XXX1 = Vector2Scale(navC.ScreenToWPCMStatic({(float)GetScreenWidth(), (float)GetScreenHeight()}, navC.camera, navC.pixPr_cm), navC.pixPr_cm); // Screen to world of screen limit

    // Anything top of and to the left of XX0 is out of bounds
    // Anything bottom of and to the right of XX1 is out of bounds
    // If XX0 is on the screen or XX1 is on the screen, the we need to draw bounds
    // Notice that we are on camera drawing mode in canvas, and therefore need to calculate screen 0,0 and width,height in world space
    if (XX0.y > 0)
    {
        DrawRectangle(XXX0.x, XXX0.y - 1, XXX1.x - XXX0.x, -XXX0.y + 1, OLIVE_GREEN_DARK);
    }
    if (XX0.x > 0)
    {
        DrawRectangle(XXX0.x - 1, XXX0.y, -XXX0.x + 1, XXX1.y - XXX0.y, OLIVE_GREEN_DARK);
    }

    if (XX1.y < (float)GetScreenHeight())
    {
        DrawRectangle(XXX0.x, limBWCP, XXX1.x - XXX0.x, XXX1.y - limBWCP, OLIVE_GREEN_DARK);
    }
    if (XX1.x < (float)GetScreenWidth())
    {
        DrawRectangle(limRWCP, XXX0.y, XXX1.x - limRWCP, XXX1.y - XXX0.y, OLIVE_GREEN_DARK);
    }
}

bool Canvas::IsGridFree(const ComponentBluePrint &blueprint, int cellX, int cellY)
{
    int x0 = std::max(0, cellX);    // inclusive
    int y0 = std::max(0, cellY);    // // inclusive
    int x1 = x0 + blueprint.Width;  // Exclusive
    int y1 = y0 + blueprint.Height; // Exclusive
    if (x1 >= (int)GridWidth || y1 >= (int)GridHeight)
        return false;

    for (int x = x0; x < x1; ++x)
    {
        for (int y = y0; y < y1; ++y)
        {

            if ((0 != gridContentInfo[GetGridIdxAtCell(x, y)].componentId) || (0 != gridContentInfo[GetGridIdxAtCell(x, y)].networkId))
                return false;
        }
    }
    return true;
}

// Sets grid cell values, notice, this fellow do not perform boundary checks and assumes the where checed elsewhere
void Canvas::SetGridCellValues(const ComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, std::uint8_t id)
{
    int x0 = std::max(0, (int)cellX); // inclusive
    int y0 = std::max(0, (int)cellY); // // inclusive
    int x1 = x0 + blueprint.Width;    // Exclusive
    int y1 = y0 + blueprint.Height;   // Exclusive

    for (int x = x0; x < x1; ++x)
    {
        for (int y = y0; y < y1; ++y)
        {
            gridContentInfo[GetGridIdxAtCell(x, y)].componentId = id;
        }
    }
}

NetworkConstraintFlags Canvas::GetNetworkConstraints(const CellPosition& anchor) const
{
    return networkConstraints[GetGridIdxAtCell(anchor.x, anchor.y)];
}


void Canvas::SetConstraints(const ComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, NetworkConstraintFlags constraintFlags)
{
    int x0 = std::max(0, (int)cellX); // inclusive
    int y0 = std::max(0, (int)cellY); // // inclusive
    int x1 = x0 + blueprint.Width;    // Exclusive
    int y1 = y0 + blueprint.Height;   // Exclusive

    for (int x = x0; x < x1; ++x)
    {
        for (int y = y0; y < y1; ++y)
        {
            networkConstraints[GetGridIdxAtCell(x, y)] = constraintFlags;
        }
    }

    for (auto specIn : blueprint.inputDataTypes)
    {
        networkConstraints[GetGridIdxAtCell(x0, y0+specIn.row)] = (NetworkConstraintFlags)(constraintFlags & ~(FLAG_NCONSTRAINT_E | FLAG_NCONSTRAINT_W));
        if (x0>1)
            networkConstraints[GetGridIdxAtCell(x0-1, y0+specIn.row)] = (NetworkConstraintFlags)(FLAG_NCONSTRAINT_N | FLAG_NCONSTRAINT_S);
    }

    if (blueprint.outputDataType.type != DT_NONE)
    {
        auto specOut = blueprint.outputDataType;
        networkConstraints[GetGridIdxAtCell(x1-1, y0+specOut.row)] = (NetworkConstraintFlags)(constraintFlags & ~(FLAG_NCONSTRAINT_E | FLAG_NCONSTRAINT_W));
        if (x1<255)
            networkConstraints[GetGridIdxAtCell(x1, y0+specOut.row)] = (NetworkConstraintFlags)(FLAG_NCONSTRAINT_N | FLAG_NCONSTRAINT_S);
    }

    // Add corner contraints to ensure we cannot create a diagonal blocking the object
    if (y0>1)
        networkConstraints[GetGridIdxAtCell(x0, y0-1)] = (NetworkConstraintFlags)((FLAG_NCONSTRAINT_SE | FLAG_NCONSTRAINT_NW));
    if (y1<255)
        networkConstraints[GetGridIdxAtCell(x0, y1)] = (NetworkConstraintFlags)((FLAG_NCONSTRAINT_SE | FLAG_NCONSTRAINT_NW));

}

// Transform a sequence of network anchor point into all the sections in the grid
std::vector<CellPosition> Canvas::GetNetworkSamplePositions(std::vector<CellPosition> &anchorPoints)
{
    if (anchorPoints.size() < 2)
    {
        return {};
    }

    std::vector<CellPosition> result;
    int nminone = anchorPoints.size() - 1;
    for (int i = 0; i < nminone; ++i)
    {
        CellPosition p0 = anchorPoints[i];
        CellPosition p1 = anchorPoints[i + 1];
        int dx = p0.x == p1.x ? 0 : p0.x > p1.x ? -1
                                                : 1;
        int dy = p0.y == p1.y ? 0 : p0.y > p1.y ? -1
                                                : 1;
        int n = std::max(std::abs(p0.x - p1.x), std::abs(p0.y - p1.y));
        for (int j = 0; j < n; ++j)
        {
            result.push_back({p0.x, p0.y});
            // Increment position for next check
            p0.x += dx;
            p0.y += dy;
        }
        if (i == nminone - 1)
        {
            result.push_back({p1.x, p1.y});
        }
    }
    return result;
}

// This should be in the networkDrawingManager, canvas should just update the cells.
NetworkCheckState Canvas::CheckNetwork(std::vector<CellPosition> &samples, std::uint8_t id)
{
    if (samples.size() < 2)
    {
        return {false};
    }

    // Check we are not overlapping other networks or crossing components (components or network anchors can be at ends)

    CellPosition ps = samples[0];
    CellPosition pn = samples[samples.size() - 1];

    // Check what we are connecting to, if anything, in the ends
    GridContentInfo startInfo = gridContentInfo[GetGridIdxAtCell(ps.x, ps.y)];
    GridContentInfo endInfo = gridContentInfo[GetGridIdxAtCell(pn.x, pn.y)];

    // if we do not have specified id, but are starting or stopping in a network node with an id,
    // Use that id, if it is two different networks, then we cannot join
    // In theory we could join if only one have an input, and output nodes have same data type, but for now I will not check for that
    if (id == 0 && startInfo.networkId != 0)
        id = startInfo.networkId;
    if (id == 0 && endInfo.networkId != 0)
        id = endInfo.networkId;

    // Now we can loop over all point and check
    // 1) Network id must be equal to input id, if input id is != 0
    // 2) There should only be allowed a component in the beginning or end of the network
    // 3) Component input/output should be compatible
    // 4) There should only be allowed networks in the beggining or end of the network
    // 5) We need to add special check when moving diagonal as we might miss other diagonals

    for (int i = 0; i < samples.size() - 1; ++i)
    {
        GridContentInfo cposInfo = gridContentInfo[GetGridIdxAtCell(samples[i].x, samples[i].y)];
        if (id != 0 && (cposInfo.networkId != 0 && cposInfo.networkId != id))
        {
            return {false};
        }

        // If there is a component except at extremes, this is not valid
        if (cposInfo.componentId != 0 && i != 0 && i != (samples.size() - 1))
        {
            return {false};
        }

        // If there is a network except at extremes, this is not valid
        if (cposInfo.networkId != 0 && i != 0 && i != (samples.size() - 1))
        {
            return {false};
        }
    }
    return {true, id, startInfo, endInfo};
}

bool Canvas::AddNetworkSegment(std::vector<CellPosition> &anchorPoints, std::uint8_t id)
{
    if (anchorPoints.size() == 0)
    {
        return false;
    }

    if (id == 0 && avaliableNetworkKeys.empty())
    {
        return false;
    }

    std::vector<CellPosition> samples = GetNetworkSamplePositions(anchorPoints);
    auto checkInfo = CheckNetwork(samples, id);
    id = checkInfo.finalId;

    if (!checkInfo.IsValid)
        return false;

    if (id == 0)
    {
        id = *(avaliableNetworkKeys.begin());
        avaliableNetworkKeys.erase(id);
    }

    // Update map, this assumes lines are either horizontal, vertical or 45 degress

    for (int i = 0; i < samples.size(); ++i)
    {
        gridContentInfo[GetGridIdxAtCell(samples[i].x, samples[i].y)].networkId = id;
    }

    // store anchore points
    networks.push_back(std::vector<CellPosition>(anchorPoints));

    // std::vector<CellPosition> overlapGridPositions = GetNetworkSamplePositions(anchorPoints);
    // std::cout << "Positions: [" << std::endl;
    // for (int i = 0; i < overlapGridPositions.size(); ++i)
    // {
    //     std::cout << overlapGridPositions[i].x << "," << overlapGridPositions[i].y << std::endl;
    // }
    return true;
}

GridContentInfo Canvas::GetCellInfo(CellPosition cp) const
{
    return GetCellInfo(cp.x, cp.y);
}
GridContentInfo Canvas::GetCellInfo(int cellX, int cellY) const
{
    if (cellX < 0 || cellY < 0 || cellX > 255 || cellY > 255)
        return {0, 0};
    return gridContentInfo[GetGridIdxAtCell(cellX, cellY)];
}

bool Canvas::HaveComponent(std::uint8_t id)
{
    return inUseComponentKeys.find(id) != inUseComponentKeys.end();
}

CanvasComponentBase *Canvas::GetComponent(std::uint8_t id)
{
    if (HaveComponent(id))
    {
        return components[id].get();
    }
    return nullptr;
}

int Canvas::GetGridIdxAtCell(int cellX, int cellY) const
{
    return cellX + cellY * GridWidth;
}

void Canvas::DrawNetworkSegment(const std::vector<CellPosition> &network, bool drawNodes, float pixPr_cm, Color c)
{
    DrawLineStripCellPos(&(network[0]), network.size(), c, pixPr_cm);
    if (drawNodes)
    {
        for (int i = 0; i < network.size(); ++i)
        {
            DrawCircleLines(network[i].x * pixPr_cm + pixPr_cm / 2.0f, network[i].y * pixPr_cm + pixPr_cm / 2.0f, pixPr_cm / 10.0f, c);
        }
    }
}

Canvas::~Canvas()
{
    // It is unloaded when window is closed, or engine is closed, not sure,
    // but unloading manually throws an exception
    // UnloadTexture(cameraTexture);
}
