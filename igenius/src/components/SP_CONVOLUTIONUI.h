#pragma once
#include "components/Component.h"
#include "components/UI_RadialSP_ConvolutionOperationMenu.h"

namespace UI_Components
{
    class SP_CONVOLUTIONUI : public UI_Component
    {
    public:
        SP_CONVOLUTIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);
        virtual void Draw(const RenderContext &) const override;
        virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx) override;
        virtual bool HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx) override;
        virtual void FocusedDraw(const RenderContext &) const override;

    private:
        UI_RadialSPConvolutionperationMenu menu;
    };
}