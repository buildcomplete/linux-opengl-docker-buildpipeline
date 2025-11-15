#pragma once
#include "canvas/tools/CanvasToolBase.h"
class PickAndPlaceComponentTool : public CanvasToolBase
{
public:
    PickAndPlaceComponentTool(Canvas &canvas);
    virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const override;
    virtual std::unique_ptr<ICommand> HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx) override;

private:
    CanvasComponentBase* pickedComponent = nullptr;
    Vector2 pickOffsetCm = {0};
    CellPosition getAnchorDropPositionGivenOffset( const NavigationContext &navCtx) const;
};