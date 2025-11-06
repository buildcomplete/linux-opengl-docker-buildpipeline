#include "PickAndPlaceComponentTool.h"
#include "raylib.h"

PickAndPlaceComponentTool::PickAndPlaceComponentTool(Canvas *c) : CanvasToolBase(c)
{
}

void PickAndPlaceComponentTool::Draw(const RenderContext &rc, const NavigationContext &navCtx) const
{
    DrawCircle(navCtx.mousePosWorldPixels.x, navCtx.mousePosWorldPixels.y, 5, YELLOW);
}

void PickAndPlaceComponentTool::HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx)
{
}
