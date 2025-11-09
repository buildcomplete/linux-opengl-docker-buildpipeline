#include "canvas/tools/DrawNetworkTool.h"
#include <cmath>
#include <raymath.h>
#include "IG_types.h"
#include <iostream>
#include <algorithm>

#include <unordered_set>
#include <cassert>
#include "canvas/Canvas.h" // Used for drawing network segments on canvas

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

struct SearchNode
{
    CellPosition position;
    SearchNode *previous;
    std::int8_t entryDir;
    SearchNode(CellPosition pos_, SearchNode *pre_, std::int8_t entryDir_)
        : position(pos_), previous(pre_), entryDir(entryDir_)
    {
    }
};

const std::int8_t CP_NDIRS = 8;
const CellPosition CP_Directions[CP_NDIRS]{
    {0, -1},  // N (0)
    {1, -1},  // NW (1)
    {1, 0},   // W (2)
    {1, 1},   // SW (3)
    {0, 1},   // S (4)
    {-1, 1},  // SE (5)
    {-1, 0},  // E (6)
    {-1, -1}, // NE (7)
};
// Calc optimal direction assuming directions of CP_Directions
std::int8_t calcOptimalDirIdx(const CellPosition &a, const CellPosition &b)
{
    auto delta = b - a;
    int dy = delta.y == 0 ? 0 : delta.y > 0 ? 1
                                            : -1;
    int dx = delta.x == 0 ? 0 : delta.x > 0 ? 1
                                            : -1;

    for (std::int8_t i = 0; i < CP_NDIRS; ++i)
    {
        if (CP_Directions[i].x == dx && CP_Directions[i].y == dy)
            return i;
    }

    // No prefered direction (should never happen)
    return -1;
};

DrawNetworkTool::DrawNetworkTool(Canvas &c) : CanvasToolBase(c)
{
    SetConstraintFunction((ConstraintFunction)[this](const CellPosition p)
    { 
        return this->canvas.GetNetworkConstraints(p);
    });
}

void DrawNetworkTool::HandleEventsAndTime(const StateContext &sc, const NavigationContext &navCtx)
{
    if (sc.DidEnterState(INPUT_STATE_FLAGS::IG_INPUT_MODE_DRAW_NETWORK))
    {
        StartDrawing();
    }

    if (sc.DidExitState(IG_INPUT_MODE_DRAW_NETWORK))
    {
        CompleteDrawing();
    }

    if (sc.DidEnterState(IG_INPUT_TRY_COMMAND))
    {
        if ( AddAnchorResultState::COMPLETE_SEGMENT == AddAnchorPoint(navCtx) )
        {
            auto n = CompleteDrawing();
            canvas.AddNetworkSegment(n, 0);
            StartDrawing();
        }
    }

    if (lastPathTo == navCtx.mousePosWorldGrid)
        return;

    if (constraintFunction != nullptr && constraintFunction(navCtx.mousePosWorldGrid) == NetworkConstraintFlags::FLAG_NCONSTRAINT_ALLDIR)
        return;

    auto pixPr_cm = navCtx.pixPr_cm;
    // Network will be arranged on grid centers
    // If there are 0 or 1 neighbours, draw a circle
    // Then draw a line from center of this to each neighbour
    // But this is only the drawing function, so here we only draw connection to other network, and it is not possible to create illigal connections
    if (drawnNetwork.size() != 0)
    {
        CellPosition toC = navCtx.mousePosWorldGrid;
        CellPosition fromC = drawnNetwork.back();
        lastPathTo = toC;

        if (fromC != toC)
        {
            std::stack<CellPosition> solution;
            if (TryContinuePathToAnchorPoints(toC, solution))
            {
                pathInProgress.clear();
                pathInProgress.push_back(fromC);
                while (!solution.empty())
                {
                    pathInProgress.push_back(solution.top());
                    solution.pop();
                }
            }
        }
    }
}

void DrawNetworkTool::SetConstraintFunction(ConstraintFunction f)
{
    constraintFunction = f;
}

void DrawNetworkTool::Draw(const RenderContext &rndrCtx, const NavigationContext &navCtx) const
{
    auto pixPr_cm = navCtx.pixPr_cm;
    Color circleColor = ELECTRIC_BLUE;
    circleColor.a = 155;
    DrawCircle(navCtx.mousePosWorldGrid.x * pixPr_cm + pixPr_cm / 2.0f, navCtx.mousePosWorldGrid.y * pixPr_cm + pixPr_cm / 2.0f, pixPr_cm / 6.0f, circleColor);

    bool completing = drawnNetwork.size() > 0 && navCtx.mousePosWorldGrid == drawnNetwork.back();

    if (drawnNetwork.size() > 0)
    {
        Canvas::DrawNetworkSegment(drawnNetwork, true, pixPr_cm, completing?NETGREEN:PURPLE);
        if (!completing)
        {
            auto XX = drawnNetwork.back();
            DrawCircle(XX.x * pixPr_cm + pixPr_cm / 2.0f, XX.y * pixPr_cm + pixPr_cm / 2.0f, pixPr_cm / 6.0f, NETGREEN);
        }
    }
    if (pathInProgress.size() > 0 && !completing)
    {
        Canvas::DrawNetworkSegment(pathInProgress, true, pixPr_cm, ELECTRIC_BLUE);
    }
}

bool DrawNetworkTool::CanAddToNetwork(CellPosition toC) const
{
    return InsideBounds(toC) && 
        (
            (drawnNetwork.size() == 0 && (constraintFunction==nullptr || constraintFunction(toC) !=  NetworkConstraintFlags::FLAG_NCONSTRAINT_ALLDIR ))
            || (drawnNetwork.size() > 0 && CanAddToNetwork(drawnNetwork.back(), toC))
        );
}
bool DrawNetworkTool::CanAddToNetwork(CellPosition fromC, CellPosition toC) const
{
    Vector2 to = {(float)toC.x, (float)toC.y};
    Vector2 from = {(float)fromC.x, (float)fromC.y};
    // Only allow 0,45,90,.. degrees etc.
    Vector2 distance = Vector2Subtract(from, to);
    bool horzOrVert = abs(distance.x) < 0.1 || abs(distance.y) < 0.1;
    bool diagonal = abs(abs(distance.x) - abs(distance.y)) < 0.1 || abs(abs(distance.y) - abs(distance.x)) < 0.1;
    bool distOk = Vector2DistanceSqr(from, to) > 0.1; // line non zero length
    return InsideBounds(fromC) && InsideBounds(toC) && distOk && (horzOrVert || diagonal);
}

void DrawNetworkTool::StartDrawing()
{
    drawnNetwork.clear();
}

std::vector<CellPosition> DrawNetworkTool::CompleteDrawing()
{
    std::cout << "NetworkManager::CompleteDrawing()" << std::endl;
    auto drawnNetworkCp = std::vector<CellPosition>(drawnNetwork);
    drawnNetwork.clear();
    pathInProgress.clear();

    if (drawnNetworkCp.size() < 2)
        drawnNetworkCp.clear();

    return drawnNetworkCp;
}

AddAnchorResultState DrawNetworkTool::AddAnchorPoint(const NavigationContext &frameCoord)
{
    std::cout << " NetworkManager::AddAnchorPoint(...)" << std::endl;
    // if starting a new network, update networkValidToHelper
    // if not starting a new it should be valid from mouse moving
    if (drawnNetwork.size() == 0)
    {
        if (TryAddAnchorPoint(frameCoord.mousePosWorldGrid))
        {
            return AddAnchorResultState::ADD_OK;
        }
    }
    // We click again the last element, complete this drawing.
    else if (drawnNetwork.back() == frameCoord.mousePosWorldGrid)
    {
        return AddAnchorResultState::COMPLETE_SEGMENT;
    }
    else
    {
        if (TryCreatePathToAnchorPoint(frameCoord.mousePosWorldGrid))
        {
            return AddAnchorResultState::ADD_OK;
        }
    }
    return AddAnchorResultState::ADD_FAILED;
}

// Tryies to add an anchor point,
// if requested position is blocked or a path cannot be found from previus ancor return false
// if point could be added, returns true
bool DrawNetworkTool::TryAddAnchorPoint(CellPosition anchor)
{
    if (!CanAddToNetwork(anchor))
        return false;

    drawnNetwork.push_back(anchor);
    return true;
}

bool DrawNetworkTool::TryCreatePathToAnchorPoint(CellPosition target)
{
    // If we can just add a straigt line, no reason to start path finding
    if (drawnNetwork.size() == 0)
    {
        return TryAddAnchorPoint(target);
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
bool DrawNetworkTool::TryContinuePathToAnchorPoints(
    CellPosition target,
    std::stack<CellPosition> &path) const
{
    if (!InsideBounds(target) || drawnNetwork.size() == 0)
        return false;

    // We could not just add, start path finding.
    CellPosition from = drawnNetwork.back();

    // if there are more than two entries, then start with same direction as last insertion
    std::int8_t startDir = drawnNetwork.size() < 2 ? -1 : calcOptimalDirIdx(drawnNetwork[drawnNetwork.size() - 2], drawnNetwork[drawnNetwork.size() - 1]);

    return TryCreatePathBetweenAnchorPoints(
        SearchNode(from, nullptr, static_cast<std::int8_t>(startDir)),
        target,
        path);
}

// Checks if a cell is inside bounds of grid [0 0;255 255]
// There are lots logic tied to bounds being inside a byte
// if changed there will be several areas to check
bool DrawNetworkTool::InsideBounds(const CellPosition &n)
{
    return n.x >= 0 && n.y >= 0 && n.x < 256 && n.y < 256;
}

bool DrawNetworkTool::TryCreatePathBetweenAnchorPoints(
    SearchNode start, // Nodes to visit including links back to previous items
    CellPosition target,
    std::stack<CellPosition> &path) const
{
    std::unordered_set<SearchPos> planned;

    std::queue<SearchNode *> nodes;
    nodes.push(&start);
    auto cf = constraintFunction;

    auto isDirAllowedForNode = [&cf](const CellPosition &n, std::int8_t dir)
    {
        return (cf == nullptr || (cf(n) & (1 << dir)) == FLAG_NCONSTRAINT_NO_CONSTRAINTS);
    };

    auto isNewAndValid = [&planned, &nodes, &isDirAllowedForNode](const CellPosition &n, std::int8_t dir)
    {
        if (InsideBounds(n) && isDirAllowedForNode(n, dir))
        {
            // Check that new neighbour is inside bounds and not already visisited
            if (planned.find({n, dir}) == planned.end())
            {
                return true;
            }
        }

        return false;
    };

    std::vector<std::unique_ptr<SearchNode>> db;
    auto addIfNewAndValid = [&planned, &nodes, &db, &isNewAndValid](const CellPosition &n, std::int8_t dir, SearchNode *parrent)
    {
        if (isNewAndValid(n, dir))
        {
            db.emplace_back(std::make_unique<SearchNode>(n, parrent, dir));
            SearchNode *nn = db.back().get();
            nodes.push(nn);
            planned.insert({n, dir});
        }
    };

    while (!nodes.empty())
    {
        SearchNode *sp = nodes.front();
        nodes.pop();

        // If we now are a target, return path.
        if (sp->position == target)
        {
            SearchNode *backTrack = sp;
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

        // Only allow same direction +-1 (except for first point)
        // if we are not at target, add all neighbours neighbours that we did not already visit.
        if (sp->entryDir == -1)
        {
            int optDir = calcOptimalDirIdx(sp->position, target);
            if (isDirAllowedForNode(sp->position, optDir))
            {
                addIfNewAndValid(CP_Directions[optDir] + sp->position, optDir, sp);
            }
            // Calculate optimal direction to begin with for first point
            for (std::int8_t i = 0; i < CP_NDIRS; ++i)
            {
                if (isDirAllowedForNode(sp->position, i))
                {
                    addIfNewAndValid(CP_Directions[i] + sp->position, i, sp);
                }
            }
        }
        else
        {
            std::int8_t nDir1 = (sp->entryDir - 1 + CP_NDIRS) % CP_NDIRS;
            std::int8_t nDir2 = (sp->entryDir + 1 + CP_NDIRS) % CP_NDIRS;

            std::int8_t optDir = calcOptimalDirIdx(sp->position, target);
            if (isDirAllowedForNode(sp->position, optDir)
                && (optDir == sp->entryDir || optDir == nDir1 || optDir == nDir2))
            {
                addIfNewAndValid(CP_Directions[optDir] + sp->position, optDir, sp);
            }

            // prefer same direction, add +-45 degrees
            addIfNewAndValid(CP_Directions[sp->entryDir] + sp->position, sp->entryDir, sp);
            addIfNewAndValid(CP_Directions[nDir1] + sp->position, nDir1, sp);
            addIfNewAndValid(CP_Directions[nDir2] + sp->position, nDir2, sp);
        }
    }

    return false;
}
