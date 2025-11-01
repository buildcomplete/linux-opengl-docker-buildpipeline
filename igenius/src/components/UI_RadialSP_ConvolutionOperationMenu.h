#pragma once

#include "components/Component.h"
#include "components/UI_RadialMenuBase.h"

class UI_RadialSPConvolutionperationMenu : public UI_RadialMenuBase
{
public:

    UI_RadialSPConvolutionperationMenu(ClickHandler clickHandler);
    //virtual void HandleClick(int idx) override;
    const std::vector<std::string> SegmentOperations = {"Edge", "Box 3", "Box 5", "Gauss 3", "Gauss 5", "Laplacian 1D" };

protected:
    virtual void DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const override;
};
