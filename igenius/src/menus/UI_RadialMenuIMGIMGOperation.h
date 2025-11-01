#pragma once

#include "menus/UI_RadialMenuBase.h"

class UI_RadialMenuIMGIMGOperation : public UI_RadialMenuBase
{
public:
    UI_RadialMenuIMGIMGOperation(ClickHandler clickHandler);
    const std::string SegmentOperations = "+-/x";

protected:
    virtual void DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const override;
};
