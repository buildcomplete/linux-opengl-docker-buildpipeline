#pragma once

#include "raylib.h"
#include "IG_types.h"
#include <vector>
#include <unordered_set>
#include <stack>
#include "context/StateContext.h"
#include "context/NavigationContext.h"
#include "context/RenderContext.h"
#include "canvas/tools/CanvasToolBase.h"

struct SearchNode;
struct SearchPos;

typedef enum : std::uint8_t
{
    ADD_FAILED,
    ADD_OK,
    COMPLETE_SEGMENT,
} AddAnchorResultState;

/**
 * Responsible for drawing network connections and handling mouse events
 * for network creation, interacting with the Canvas to understand item presence.
 */
class DrawNetworkTool : public CanvasToolBase
{
public:
    DrawNetworkTool(Canvas &c);

    std::unique_ptr<ICommand>  HandleEventsAndTime(const StateContext &sc, const NavigationContext &navCtx);
    void Draw(const RenderContext &rc, const NavigationContext &navC) const;
    
    
    bool CanAddToNetwork(CellPosition toC) const;
    bool CanAddToNetwork(CellPosition fromC, CellPosition toC) const;
    void StartDrawing();
    std::vector<CellPosition> CompleteDrawing();
    AddAnchorResultState AddAnchorPoint(const NavigationContext &frameCoord);
    bool TryAddAnchorPoint(CellPosition anchor);
    bool TryCreatePathToAnchorPoint(CellPosition target);
    bool TryContinuePathToAnchorPoints(CellPosition target, std::stack<CellPosition> &path) const;
    static bool InsideBounds(const CellPosition &n);
    void SetConstraintFunction(ConstraintFunction f);

private:
    std::vector<CellPosition> drawnNetwork = std::vector<CellPosition>(0);
    std::vector<CellPosition> pathInProgress = std::vector<CellPosition>(0);
    CellPosition lastPathTo;
    ConstraintFunction constraintFunction = nullptr;

    bool TryCreatePathBetweenAnchorPoints(
        SearchNode start,
//        std::unordered_set<SearchPos> &visited,
        CellPosition target,
        std::stack<CellPosition> &path) const;
};
