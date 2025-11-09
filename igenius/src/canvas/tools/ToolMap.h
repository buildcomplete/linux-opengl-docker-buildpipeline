#pragma once
#include "canvas/tools/CanvasToolBase.h"

// Maps INPUT_STATE to a specific canvas tool
class ToolMap
{
public:
    ToolMap(Canvas& c);
    CanvasToolBase* GetTool(INPUT_STATE_FLAGS f);

private:
    std::vector<std::unique_ptr<CanvasToolBase>> tools;
};