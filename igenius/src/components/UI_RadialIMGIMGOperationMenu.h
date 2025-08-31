#pragma once

#include "components/Component.h"

class UI_RadialIMGIMGOperationMenu : public UI_RadialMenuDrawingComponent
{
public:
    UI_RadialIMGIMGOperationMenu();
    virtual void HandleClick(int idx) override;
    const std::string SegmentOperations = "+-/x";

protected:
    virtual void DrawSegmentIcon(const RenderContext &rc, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const override;
};
