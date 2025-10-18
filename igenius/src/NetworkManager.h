#ifndef ENGINE_NETWORKDRAWINGMANAGER_H
#define ENGINE_NETWORKDRAWINGMANAGER_H

#include "raylib.h"
#include "IG_types.h"
#include <vector>
#include "Canvas.h"
#include <unordered_set>
#include <stack>

struct SearchNode;
struct SearchPos;

/**
 * Responsible for drawing network connections and handling mouse events
 * for network creation, interacting with the Canvas to understand item presence.
 */
class NetworkManager
{
public:
    void HandleEvents(const StateContext &sc, const NavigationContext &navCtx);
    void Draw(const RenderContext &rc, const NavigationContext &navC) const;
    bool CanAddToNetwork(CellPosition toC);
    bool CanAddToNetwork(CellPosition fromC, CellPosition toC);
    void StartDrawing();
    std::vector<CellPosition> CompleteDrawing();
    void AddAnchorPoint(const NavigationContext &frameCoord);
    bool TryAddAnchorPoint(CellPosition anchor);
    bool TryCreatePathToAnchorPoint(CellPosition target);
    bool TryContinuePathToAnchorPoints(CellPosition target, std::stack<CellPosition> &path) const;
    static bool InsideBounds(const CellPosition &n);
    void SetConstraintFunction(ConstraintFunction f);

private:
    std::vector<CellPosition> drawnNetwork = std::vector<CellPosition>(0);
    std::vector<CellPosition> pathInProgress = std::vector<CellPosition>(0);
    bool isInDrawMode = false;
    CellPosition lastPathTo;
    ConstraintFunction constraintFunction = nullptr;

    bool TryCreatePathBetweenAnchorPoints(
        SearchNode start,
//        std::unordered_set<SearchPos> &visited,
        CellPosition target,
        std::stack<CellPosition> &path) const;
};

#endif