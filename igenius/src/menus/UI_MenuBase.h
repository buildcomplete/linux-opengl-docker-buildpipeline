#pragma once
#include "context/NavigationContext.h"
#include "context/RenderContext.h"
#include "context/StateContext.h"
#include <functional>

typedef enum : std::uint8_t
{
    UITEM_CLOSED,
    UITEM_EXPANDING,
    UITEM_OPEN,
    UITEM_CLOSING
} UI_ANIM_STATE;

class UI_MenuBase
{
public:
    using ClickHandler = std::function<void(int segmentIndex)>;

    UI_MenuBase(ClickHandler clickHandler_);

    virtual void HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx);
    virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const = 0; // Draws the components
    void BeginShow(Vector2 origo);                                                              // Start expanding a menu item.
    void BeginHide();                                                                           // Start closing a menu item.
    bool IsVisible();

protected:
    double transitionTime = .25; // time in second for animations
    double transitionStartTime;
    float expansionProgress = 0;

    UI_ANIM_STATE animState = UI_ANIM_STATE::UITEM_CLOSED;
    Vector2 origoCM = {0};
    ClickHandler onClickHandler;

    void HandleClickInternal(int idx); // Called when a menu is clicked, fires the clickHandler
};
