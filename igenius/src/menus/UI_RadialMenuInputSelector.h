#pragma once

#include "menus/UI_RadialMenuBase.h"

class UI_RadialMenuInputSelector : public UI_RadialMenuBase
{
public:

    UI_RadialMenuInputSelector(ClickHandler clickHandler);
    //virtual void HandleClick(int idx) override;
    const std::vector<std::string> SegmentOperations = {"SELECT", "NET", "PLACE", "MOVE", "DELETE"};

protected:
    virtual void DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const override;
};
