#pragma once
#include "menus/UI_MenuBase.h"

struct RadialMenuSegment
{
    Color edgeColor;
    Color backgroundColor;
    Color hoverColor;
    int yourId; // to identify segment in callback so you don't have to rely on index
};

class UI_RadialMenuBase : public UI_MenuBase
{
public:
    using ClickHandlerRadialMenu = std::function<void(int segmentIndex, int segmentId)>;

    UI_RadialMenuBase(ClickHandlerRadialMenu clickHandler_, std::vector<RadialMenuSegment> segments, float rOutCm, float rInCm);

    // Returns the calculated segment index, -1 if nothing is selected
    int GetHoverSegment(const NavigationContext &navC) const;
    virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const override;

    virtual void DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const = 0;
    virtual void HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx) override;

protected:
    std::vector<RadialMenuSegment> segments;
    float rOutCm;
    float rInCm;
    int hoveredSegment = -1;
    int selectedSegment = -1;

private:
    void DrawRadialMenu(const RenderContext &rc, const NavigationContext &navCtx, int segments) const;
};
