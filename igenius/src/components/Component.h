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
protected:
    double transitionTime = .25; // time in second for animations
    double transitionStartTime;
    float expansionProgress = 0;

    UI_ANIM_STATE animState = UI_ANIM_STATE::UITEM_CLOSED;
    Vector2 origoCM = {0};

public:
    virtual void HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx);
    virtual void Draw(const NavigationContext &navC, const RenderContext &rc) = 0; // Draws the components
    virtual void HandleClick(int idx) = 0; // Called when a meny is clicked, to be implemented in actual menu, 
    void Show(Vector2 origo);                                                           // Start expanding a menu item.
    void Hide();                                                           // Start closing a menu item.
    bool IsVisible();
};

struct RadialMenuSegmentColors
{
    Color edgeColor;
    Color backgroundColor;
    Color hoverColor;
};

class UI_RadialMenuDrawingComponent : public UI_Selector
{
public:
    UI_RadialMenuDrawingComponent(std::vector<RadialMenuSegmentColors> segments, float rOutCm, float rInCm);

    // Returns the calculated segment index, -1 if nothing is selected
    int GetHoverSegment(const NavigationContext &navC);
    virtual void Draw(const NavigationContext &navC, const RenderContext &rc) override;
    
    virtual void DrawSegmentIcon(const NavigationContext &navC, const RenderContext &rc, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) = 0;
    virtual void HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx) override;
    

protected:
    std::vector<RadialMenuSegmentColors> segments;
    float rOutCm;
    float rInCm;
    int hoveredSegment=-1;
    int selectedSegment=-1;
private:
    void DrawRadialMenu(const NavigationContext &navC, const RenderContext &rc, int segments);

};

class UI_RadialIMGIMGOperationMenu : public UI_RadialMenuDrawingComponent
{
public:
    UI_RadialIMGIMGOperationMenu();
    virtual void HandleClick(int idx) override;
    const std::string SegmentOperations = "+-/x";

protected:
    virtual void DrawSegmentIcon(const NavigationContext &navC, const RenderContext &rc, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) override;
};

#endif
