#ifndef IG_IG_COMPONENTS_H
#define IG_IG_COMPONENTS_H

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


// class Component
// {
// public:
//     std::uint8_t id = 0;

//     // Datatype in input, and wheter or not a data field is optional
//     std::vector<IOSpec> inputTypes;
//     IGDataTypes outputType;

// };

class UI_Component
{
public:
    std::uint8_t id = 0;
    CellPosition anchor = {0, 0};
    ComponentBluePrint bluePrint;

    UI_Component(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);

    virtual void Draw(const RenderContext &) const;

    Vector2 GetCursorOffset(const NavigationContext &navCtx);

    // Handles when the user start a new command over a component,
    // Thew component might or might not react to it,
    // if it react, it should return true **transfering** the mouse controll to the component
    virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx, const RenderContext &rndCtx);

    void DrawStandardComponentFrame(const RenderContext &rc, const float padding_cm) const;

protected:
    void DrawStandardSockets(const RenderContext &rc, float padding_cm) const;
};

namespace UI_Components
{
    class VirtualCameraUI : public UI_Component
    {
    public:
        VirtualCameraUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);
        virtual void Draw(const RenderContext &) const override;
        virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx, const RenderContext &rndCtx) override;
    private:
        bool isCapturing = false;
    };

    class SP_CONVOLUTIONUI : public UI_Component
    {
    public:
        SP_CONVOLUTIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);
        virtual void Draw(const RenderContext &) const override;
    };

    class IMG_IMG_IMG_OPERATIONUI : public UI_Component
    {
    public:
        IMG_IMG_IMG_OPERATIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);
        virtual void Draw(const RenderContext &) const override;
    };

    class DrawingSurfaceComponent : public UI_Component
    {
    public:
        DrawingSurfaceComponent(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_);
        virtual void Draw(const RenderContext &) const override;
        virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx, const RenderContext &rndCtx) override;
    private:
        std::vector<Vector2> strokes;
    };

}

#endif