#pragma once

#include "components/Component.h"

class UI_RadialIMGIMGOperationMenu : public UI_RadialMenuDrawingComponent
{
public:
    UI_RadialIMGIMGOperationMenu(ClickHandler clickHandler);
    const std::string SegmentOperations = "+-/x";

protected:
    virtual void DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const override;
};
