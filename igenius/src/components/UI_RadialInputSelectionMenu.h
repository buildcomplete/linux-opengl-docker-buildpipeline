#pragma once

#include "components/Component.h"

class UI_RadialInputSelectionMenu : public UI_RadialMenuDrawingComponent
{
public:

    UI_RadialInputSelectionMenu(ClickHandler clickHandler);
    //virtual void HandleClick(int idx) override;
    const std::vector<std::string> SegmentOperations = {"SELECT", "NET", "PLACE", "MOVE", "DELETE"};

protected:
    virtual void DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const override;
};
