#pragma once
#include "canvas/components/CanvasComponents.h"
#include "menus/UI_RadialSP_ConvolutionOperationMenu.h"

namespace CanvasComponents
{
    class SP_CONVOLUTIONUI : public CanvasComponentBase
    {
    public:
        SP_CONVOLUTIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);
        virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const override;
        virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx) override;
        virtual bool HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx) override;
        virtual void FocusedDraw(const RenderContext &rc, const NavigationContext &navCtx) const override;

    private:
        UI_RadialSPConvolutionperationMenu menu;
        void OnMenuSelectOperationClick(int segmentIndex);
    };
}