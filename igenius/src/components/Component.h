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
#include <functional>
#include <cstdint>



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
    virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx);
    virtual bool HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx);

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
        virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx) override;
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
        virtual bool TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx) override;
        virtual bool HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx) override;
    private:
        std::vector<Vector2> strokes;
        Vector2 currentMouseMove[2] = {0};
        bool isDrawing = false;
    };
}

typedef enum : std::uint8_t
{
    UITEM_CLOSED,
    UITEM_EXPANDING,
    UITEM_OPEN,
    UITEM_CLOSING
} UI_ANIM_STATE;

class UI_Selector
{
private:
	double transitionTime = 1.0; // time in second for open/close animation
	double startExpansionTime;
	UI_ANIM_STATE animState = UI_ANIM_STATE::UITEM_CLOSED;
public:
	virtual void Draw(); // Draws the components
	virtual void BeginExpand(); // Start expanding a menu item.
};

class UI_RadialMenu : public UI_Selector
{

protected:
    int DrawRadialMenu(const Vector2& center, float radiusStart, float radiusEnd, std::vector<std::function<void()>> menuDrawingFunctions);
};

class UI_RadialMenuConvolution
{

};

class UI_RadialMenuDrawingComponent : public UI_Selector
{
	virtual void Draw();
};

#endif
