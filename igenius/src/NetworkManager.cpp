#include "NetworkManager.h"
#include <cmath>
#include <raymath.h>
#include "IG_types.h"
#include <iostream>
#include <algorithm>

#include <unordered_set>
#include <cassert>

// Search related helpers.

struct SearchPos
{
    CellPosition position;
    std::int8_t pDir;
    bool operator==(const SearchPos &other) const
    {
        return (position.x == other.position.x &&
                position.y == other.position.y &&
                pDir == other.pDir);
    }
    // Constructor
    SearchPos(CellPosition pos, std::int8_t direction)
        : position(pos), pDir(direction) {}
};
// Hash function for CellPosition to use in unordered_set
namespace std
{
    template <>
    struct hash<SearchPos>
    {
        size_t operator()(const SearchPos &pos) const
        {
            // Properly using left shifts.
            return hash<int>()(
                (pos.position.x) +
                (pos.position.y << 8) +
                (pos.pDir << 16));
        }
    };
}

struct SearchPath
{
    CellPosition position;
    SearchPath *previous;
    std::int8_t entryDir;
    SearchPath(CellPosition pos_, SearchPath *pre_, std::int8_t entryDir_)
        : position(pos_), previous(pre_), entryDir(entryDir_)
    { }
};

const std::int8_t CP_NDIRS = 8;
const CellPosition CP_Directions[CP_NDIRS]{
    { 0, -1},  // N (0)
    { 1, -1},  // NW (1)
    { 1,  0},   // W (2)
    { 1,  1},   // SW (3)
    { 0,  1},   // S (4)
    {-1,  1},  // SE (5)
    {-1,  0},  // E (6)
    {-1, -1}, // NE (7)
};
// Calc optimal direction assuming directions of CP_Directions
std::int8_t calcOptimalDirIdx(const CellPosition& a, const CellPosition& b)
{
    auto delta = b-a;
    int dy = delta.y == 0 ? 0 :
        delta.y > 0 ? 1 : -1;
    int dx = delta.x == 0 ? 0 :
        delta.x > 0 ? 1 : -1;
    
    for (std::int8_t i=0;i<CP_NDIRS;++i)
    {
        if (CP_Directions[i].x == dx && CP_Directions[i].y == dy )
            return i;
    }
 
    // No prefered direction (should never happen)
    return -1;
};

void NetworkManager::HandleEvents(const StateContext &sc)
{
}

void NetworkManager::Draw(const RenderContext &rndrCtx, const NavigationContext &navCtx, const StateContext &stateCtx)
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
        DrawCircle(navCtx.mousePosWorldGrid.x * pixPr_cm + pixPr_cm / 2.0f, navCtx.mousePosWorldGrid.y * pixPr_cm + pixPr_cm / 2.0f, pixPr_cm / 6.0f, circleColor);

        if (drawnNetwork.size() != 0)
        {
            CellPosition toC = navCtx.mousePosWorldGrid;
            CellPosition fromC = drawnNetwork.back();

            if (fromC != toC)
            {
                std::stack<CellPosition> solution;
                if (TryContinuePathToAnchorPoints(toC, solution))
                {
                    std::vector<CellPosition> dummy;
                    dummy.push_back(fromC);
                    while (!solution.empty())
                    {
                        dummy.push_back(solution.top());
                        solution.pop();
                    }
                    Canvas::DrawNetworkSegment(dummy, stateCtx, pixPr_cm, ELECTRIC_BLUE);
                }
            }
        }
    }
    if (drawnNetwork.size() > 0)
    {
        Canvas::DrawNetworkSegment(drawnNetwork, stateCtx, pixPr_cm, ELECTRIC_BLUE);
    }
}

bool NetworkManager::CanAddToNetwork(CellPosition toC)
{
    return drawnNetwork.size() == 0 || CanAddToNetwork(drawnNetwork.back(), toC);
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
        drawnNetwork.push_back(frameCoord.mousePosWorldGrid);
    }
    else
    {
        TryCreatePathToAnchorPoint(frameCoord.mousePosWorldGrid);
    }
}

// Tryies to add an anchor point,
// if requested position is blocked or a path cannot be found from previus ancor return false
// if point could be added, returns true
bool NetworkManager::TryAddAnchorPoint(CellPosition anchor)
{
    if (!CanAddToNetwork(anchor))
        return false;

    drawnNetwork.push_back(anchor);
    return true;
}

bool NetworkManager::TryCreatePathToAnchorPoint(CellPosition target)
{
    // If we can just add a straigt line, no reason to start path finding
    if (drawnNetwork.size() == 0 )
    {
        TryAddAnchorPoint(target);
        return true;
    }

    std::stack<CellPosition> solution;
    if (TryContinuePathToAnchorPoints(
            target,
            solution))
    {
        while (!solution.empty())
        {
            auto x = solution.top();
            solution.pop();
            TryAddAnchorPoint(x);
        }
        return true;
    }

    return false;
}
bool NetworkManager::TryContinuePathToAnchorPoints(
    CellPosition target,
    std::stack<CellPosition>& path) const
{
     // We could not just add, start path finding.
    CellPosition from = drawnNetwork.back();

    // if there are more than two entries, then start with same direction as last insertion
    std::int8_t startDir = drawnNetwork.size() < 2 ? 
        -1 : 
        calcOptimalDirIdx(drawnNetwork[drawnNetwork.size()-2], drawnNetwork[drawnNetwork.size()-1]);

    std::unordered_set<SearchPos> visited;
    std::queue<SearchPath> planned;
    visited.insert({from, static_cast<std::int8_t>(startDir)});
    SearchPath sp(from, nullptr, static_cast<std::int8_t>(startDir));
    return TryCreatePathBetweenAnchorPoints(
            sp,
            planned,
            visited,
            target,
            path);
}


bool NetworkManager::TryCreatePathBetweenAnchorPoints(
    SearchPath sp,                          // Actual path traversed, this will be each node and should be compressed into anchor point at direction changes
    std::queue<SearchPath> &nodes,          // Nodes to visit in order
    std::unordered_set<SearchPos> &planned, // Nodes planned to be visisted (in nodes list, or was in nodes list earlier)
    CellPosition target,
    std::stack<CellPosition>& path) const
{
    // If we now are a target, return path.
    if (sp.position == target)
    {
        SearchPath *backTrack = &sp;
        int leDir = -1;
        while (backTrack != nullptr)
        {
            // only add points when changing directions
            if (backTrack->entryDir != leDir)
            {
                path.push(backTrack->position);
                leDir = backTrack->entryDir;
            }
            
            backTrack = backTrack->previous;
        }
        return true;
    }

    auto addIfValid = [&planned, &nodes, &sp](const CellPosition &n, int dir)
    {
        if (n.x >= 0 && n.y >= 0 && n.x < 256 && n.y < 256)
        {
            // Check that new neighbour is inside bounds and not already visisited
            if (planned.find({n, dir}) == planned.end())
            {
                nodes.push(SearchPath(n, &sp, dir));
                planned.insert({n, dir});
            }
        }
    };

    // Only allow same direction +-1 (except for first point)
    // if we are not at target, add all neighbours neighbours that we did not already visit.
    if (sp.entryDir == -1)
    {
        int optDir = calcOptimalDirIdx(sp.position, target);
        addIfValid(CP_Directions[optDir] + sp.position, optDir);
        // Calculate optimal direction to begin with for first point
        for (std::int8_t i = 0; i < CP_NDIRS; ++i)
        {
            addIfValid(CP_Directions[i] + sp.position, i);
        }
    }
    else
    {
        std::int8_t nDir1=(sp.entryDir - 1 + CP_NDIRS) % CP_NDIRS;
        std::int8_t nDir2=(sp.entryDir + 1 + CP_NDIRS) % CP_NDIRS;

        std::int8_t optDir = calcOptimalDirIdx(sp.position, target);
        if (optDir == sp.entryDir || optDir == nDir1 || optDir == nDir2 )
        {
            addIfValid(CP_Directions[optDir] + sp.position, optDir);
        }

        // prefer same direction, add +-45 degrees
        addIfValid(CP_Directions[sp.entryDir] + sp.position, sp.entryDir);
        addIfValid(CP_Directions[nDir1] + sp.position, nDir1);
        addIfValid(CP_Directions[nDir2] + sp.position, nDir2);
    }

    while (!nodes.empty())
    {
        auto next = nodes.front();
        nodes.pop();

        if (TryCreatePathBetweenAnchorPoints(next, nodes, planned, target, path))
        {
            return true;
        }
    }

    return false;
}
