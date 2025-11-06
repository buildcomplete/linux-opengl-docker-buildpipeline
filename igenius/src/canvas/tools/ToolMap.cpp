#include "ToolMap.h"
#include "canvas/tools/PickAndPlaceComponentTool.h"

ToolMap::ToolMap(Canvas *c)
{
      // Create all tools for canvas
    tools.push_back(std::make_unique<PickAndPlaceComponentTool>(c));
}

CanvasToolBase *ToolMap::GetTool(INPUT_STATE_FLAGS f)
{
    if (INPUT_STATE_FLAGS::IG_INPUT_MODE_MOVE_COMPONENT & f )
        return tools[0].get();
    return nullptr;
}
