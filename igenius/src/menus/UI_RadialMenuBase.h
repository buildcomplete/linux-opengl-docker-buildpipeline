#pragma once
#include "menus/UI_MenuBase.h"

struct RadialMenuSegmentColors
{
    Color edgeColor;
    Color backgroundColor;
    Color hoverColor;
};

class UI_RadialMenuBase : public UI_MenuBase
{
public:
    UI_RadialMenuBase(ClickHandler clickHandler_, std::vector<RadialMenuSegmentColors> segments, float rOutCm, float rInCm);

    // Returns the calculated segment index, -1 if nothing is selected
    int GetHoverSegment(const NavigationContext &navC) const;
    virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const override;

    virtual void DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const = 0;
    virtual void HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx) override;

protected:
    std::vector<RadialMenuSegmentColors> segments;
    float rOutCm;
    float rInCm;
    int hoveredSegment = -1;
    int selectedSegment = -1;

private:
    void DrawRadialMenu(const RenderContext &rc, const NavigationContext &navCtx, int segments) const;
};
