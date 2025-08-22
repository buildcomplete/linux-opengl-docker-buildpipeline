#include "Canvas.h"
#include <iostream>
#include <rlgl.h>



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
        rlVertex2f(points[i].x * pixPr_cm+ pixPr_cm / 2.0f, points[i].y * pixPr_cm + pixPr_cm / 2.0f );
        rlVertex2f(points[i + 1].x * pixPr_cm+ pixPr_cm / 2.0f, points[i + 1].y * pixPr_cm+ pixPr_cm / 2.0f);
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
    cameraTexture = LoadTexture("game_camera_transparent2.png");
}


bool Canvas::AddComponent(const ComponentBluePrint& bluePrint, unsigned char cellAnchorX, unsigned char cellAnchorY)
{
    if (avaliableComponentKeys.empty())
        return false;

    // Add check if position is valid
    if (!IsGridFree(bluePrint, cellAnchorX, cellAnchorY))
        return false;

    std::uint8_t insertId = *(avaliableComponentKeys.begin());

    // Update lookup table
    SetGridCellValues(bluePrint, cellAnchorX, cellAnchorY, insertId);

    //components[insertId] = new UI_Component({insertId, {cellAnchorX, cellAnchorY}, bluePrint});
    components[insertId] = ComponentFactory::CreateUI_Component(bluePrint,insertId, {cellAnchorX, cellAnchorY});
    
    avaliableComponentKeys.erase(insertId);
    inUseComponentKeys.insert(insertId);
    return true;
}

void Canvas::Update()
{
    // Update location of mouse according to screen, gameworld in Component and network coordinates

    // Perform detection of what objects are selected
}

// Select / deselect component
// Multiple components can be selected
// We could, read the state here, and handle the logic
// Or we could manage that outside in the engine, and just inform when an event occured
// centralized or decentralized...
void ToggleComponent()
{
}

void Canvas::Draw(CoordinateHelper &coordinateHelper, bool drawAnchors)
{
    RenderContext rc = {coordinateHelper, cameraTexture};
    for (const auto& i : inUseComponentKeys)
    {
        components[i]->Draw(rc);
    }

    for (int i = 0; i < networks.size(); ++i)
    {
        Canvas::DrawNetworkSegment(networks[i], drawAnchors, coordinateHelper.pixPr_cm);
    }
}

bool Canvas::IsGridFree(const ComponentBluePrint &blueprint, int cellX, int cellY)
{
    int x0 = std::max(0, cellX);   // inclusive
    int y0 = std::max(0, cellY);   // // inclusive
    int x1 = x0 + blueprint.Width; // Exclusive
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

// Sets grid cell values, notice, this fellow do not perform boundaris checks and assumes the where checed elsewhere
void Canvas::SetGridCellValues(const ComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, std::uint8_t id)
{
    int x0 = std::max(0, (int)cellX); // inclusive
    int y0 = std::max(0, (int)cellY); // // inclusive
    int x1 = x0 + blueprint.Width;    // Exclusive
    int y1 = y0 + blueprint.Height;    // Exclusive

    for (int x = x0; x < x1; ++x)
    {
        for (int y = y0; y < y1; ++y)
        {
            gridContentInfo[GetGridIdxAtCell(x, y)].componentId = id;
        }
    }
}

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
        int dx = p0.x == p1.x ? 0 : p0.x > p1.x ? -1 : 1;
        int dy = p0.y == p1.y ? 0 : p0.y > p1.y ? -1 : 1;
        int n = std::max(std::abs(p0.x - p1.x), std::abs(p0.y - p1.y));
        for (int j = 0; j < n; ++j)
        {
            result.push_back({p0.x,p0.y});
            // Increment position for next check
            p0.x += dx;
            p0.y += dy;
        }
        if (i == nminone-1)
        {
            result.push_back({p1.x,p1.y});
        }
    }
    return result;
    
}

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

    for (int i=0;i<samples.size()-1;++i)
    {
        GridContentInfo cposInfo = gridContentInfo[GetGridIdxAtCell(samples[i].x, samples[i].y)];
        if (id != 0 && (cposInfo.networkId != 0 && cposInfo.networkId != id))
        {
            return {false};
        }

        // If there is a component except at extremes, this is not valid
        if ( cposInfo.componentId != 0 && i !=0 && i != (samples.size()-1))
        {
            return {false};
        }

        // If there is a network except at extremes, this is not valid
        if ( cposInfo.networkId != 0 && i !=0 && i != (samples.size()-1))
        {
            return {false};
        }
    }
    return {true, id, startInfo, endInfo};
}

bool Canvas::AddNetworkSegment(std::vector<CellPosition> &anchorPoints, std::uint8_t id)
{
    if (id == 0 && avaliableNetworkKeys.empty())
    {
        return false;
    }

    std::vector<CellPosition> samples = GetNetworkSamplePositions(anchorPoints);
    auto checkInfo = CheckNetwork(samples, id );
    id = checkInfo.finalId;

    if (!checkInfo.IsValid)
        return false;

    if (id == 0)
    {
        id = *(avaliableNetworkKeys.begin());
        avaliableNetworkKeys.erase(id);
    }

    // Update map, this assumes lines are either horizontal, vertical or 45 degress
    
    for (int i=0;i<samples.size();++i)
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

GridContentInfo Canvas::GetCellInfo(int cellX, int cellY) const
{
    if (cellX < 0 || cellY < 0 || cellX > 255 || cellY > 255)
        return {0, 0};
    return gridContentInfo[GetGridIdxAtCell(cellX, cellY)];
}

UI_Component* Canvas::GetComponent(int id)
{
    if (inUseComponentKeys.find(id) != inUseComponentKeys.end())
    {
        return components[id].get();
    }
    return nullptr;
}

int Canvas::GetGridIdxAtCell(int cellX, int cellY) const
{
    return cellX + cellY * GridWidth;
}

void Canvas::DrawNetworkSegment(std::vector<CellPosition> &network, bool drawNodes, float pixPr_cm)
{
    DrawLineStripCellPos(&(network[0]), network.size(), NETGREEN, pixPr_cm);
    if (drawNodes)
    {
        for (int i = 0; i < network.size(); ++i)
        {
            DrawCircleLines(network[i].x * pixPr_cm + pixPr_cm / 2.0f, network[i].y * pixPr_cm + pixPr_cm / 2.0f, pixPr_cm / 10.0f, NETGREEN);
        }
    }
}

Canvas::~Canvas()
{
    std::cout << "Destrying canvas, hope you are exiting" << std::endl;
    // It is unloaded when window is closed, or engine is closed, not sure,
    // but unloadung manually throws an exception
	//UnloadTexture(cameraTexture);
}
