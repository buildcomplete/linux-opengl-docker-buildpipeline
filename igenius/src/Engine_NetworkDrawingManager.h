#ifndef ENGINE_NETWORKDRAWINGMANAGER_H
#define ENGINE_NETWORKDRAWINGMANAGER_H

#include "raylib.h"
#include <vector>
#include "Canvas.h"

/**
 * Responsible for drawing network connections and handling mouse events 
 * for network creation, interacting with the Canvas to understand item presence.
 */
class Engine_NetworkDrawingManager
{
public:

    void HandleEvents(const StateContext& sc);
    void Draw(const RenderContext& rc, const NavigationContext& navC, const StateContext& sc);
    void StartDrawing();
    void CompleteDrawing(Canvas &canvas);
    void AddAnchorPoint(const NavigationContext &frameCoord, float pixPr_cm);

private:
    int networkDrawPosIdx = 1;
    CellPosition networkDrawPos[2] = {{0,0},{0,0}};
    CellPosition networkValidToHelper = {0,0};
    std::vector<CellPosition> drawnNetwork= std::vector<CellPosition>(5);
    bool anyNewvalidPointInNetwork = false;
};

#endif