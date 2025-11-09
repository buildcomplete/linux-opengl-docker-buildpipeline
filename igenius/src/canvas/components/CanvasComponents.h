#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <set>
#include "IG_types.h"
#include <cstdint>
#include "ComponentBluePrint.h"
#include "CoordinateHelper.h"
#include "context/NavigationContext.h"
#include "context/RenderContext.h"
#include "context/StateContext.h"
#include <functional>
#include <cstdint>

void DrawCircleSelectorLines(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color);

class CanvasComponentBase
{
 public:
    std::uint8_t id = 0;
    CellPosition anchor = {0, 0};
    ComponentBluePrint bluePrint;

    CanvasComponentBase(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);

    virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const;
    virtual void FocusedDraw(const RenderContext &rc, const NavigationContext &navCtx) const;

    Vector2 GetCursorOffset(const NavigationContext &navCtx);

    // Handles when the user start a new command over a component,
    // Thew component might or might not react to it,
    // if it react, it should return true **transfering** the mouse controll to the component
    virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx);
    virtual bool HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx);

    void DrawStandardComponentFrame(const RenderContext &rc, const NavigationContext &navCtx, const float padding_cm) const;

    // const std::vector<IOPin>& GetInputPins() const;

protected:
    void DrawStandardSockets(const RenderContext &rc, const NavigationContext &navCtx, float padding_cm) const;
    // std::vector<IOPin> inputs;
    // std::vector<IOPin> outputs;
};

namespace CanvasComponents
{
    class VirtualCameraUI : public CanvasComponentBase
    {
    public:
        VirtualCameraUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);
        virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const override;
        virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx) override;

    private:
        bool isCapturing = false;
    };

    class IMG_IMG_IMG_OPERATIONUI : public CanvasComponentBase
    {
    public:
        IMG_IMG_IMG_OPERATIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);
        virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const override;
    };

    class DrawingSurfaceComponent : public CanvasComponentBase
    {
    public:
        DrawingSurfaceComponent(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);
        virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const override;
        virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx) override;
        virtual bool HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx) override;

    private:
        std::vector<Vector2> strokes;
        Vector2 currentMouseMove[2] = {0};
        bool isDrawing = false;
    };
}
