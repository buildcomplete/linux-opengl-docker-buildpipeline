#ifndef ENGINE_NETWORKDRAWINGMANAGER_H
#define ENGINE_NETWORKDRAWINGMANAGER_H

#include "raylib.h"
#include <vector>

/**
 * Responsible for drawing network connections and handling mouse events 
 * for network creation, interacting with the Canvas to understand item presence.
 */
class Engine_NetworkDrawingManager
{
public:
    void Draw(Vector2 worldPos, bool drawMode, float pixPr_cm);
    void StartNewNetwork();
    void CompleteDrawing();
    void AddAnchorPoint(Vector2 worldPos, float pixPr_cm);

private:
    int networkDrawPosIdx = 1;
    Vector2 networkDrawPos[2] = {{0,0},{0,0}};
    Vector2 networkValidToHelper = {0,0};
    std::vector<Vector2> drawnNetwork= std::vector<Vector2>(5);
    bool anyNewvalidPointInNetwork = false;
    std::vector<std::vector<Vector2>> networks = std::vector<std::vector<Vector2>>(5); // Move to canvas, realized networks
};

#endif