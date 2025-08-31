#pragma once

#include "components/Component.h"

class UI_RadialSPConvolutionperationMenu : public UI_RadialMenuDrawingComponent
{
public:

    UI_RadialSPConvolutionperationMenu(ClickHandler clickHandler);
    //virtual void HandleClick(int idx) override;
    const std::string SegmentOperations = "123456";

protected:
    virtual void DrawSegmentIcon(const RenderContext &rc, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const override;
};
