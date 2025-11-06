#pragma once
#include "CanvasToolBase.h"
class PickAndPlaceComponentTool : public CanvasToolBase
{
public:
    PickAndPlaceComponentTool(Canvas* canvas);
    virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const;
    virtual void HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx);

};