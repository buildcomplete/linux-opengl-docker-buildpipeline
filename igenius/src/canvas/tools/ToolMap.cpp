#include "ToolMap.h"
#include "canvas/tools/PickAndPlaceComponentTool.h"
#include "canvas/tools/DrawNetworkTool.h"

ToolMap::ToolMap(Canvas &c)
{
      // Create all tools for canvas
    tools.push_back(std::make_unique<PickAndPlaceComponentTool>(c));
    tools.push_back(std::make_unique<DrawNetworkTool>(c));
}

CanvasToolBase *ToolMap::GetTool(INPUT_STATE_FLAGS f)
{
    if (INPUT_STATE_FLAGS::IG_INPUT_MODE_MOVE_COMPONENT & f )
        return tools[0].get();
    if (INPUT_STATE_FLAGS::IG_INPUT_MODE_DRAW_NETWORK & f )
        return tools[1].get();
    
    return nullptr;
}
