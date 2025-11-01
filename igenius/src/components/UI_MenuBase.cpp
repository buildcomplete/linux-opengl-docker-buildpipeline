#include "components/UI_MenuBase.h"
#include <raymath.h>

UI_MenuBase::UI_MenuBase(ClickHandler clickHandler_) 
    : onClickHandler(clickHandler_)
{
}

void UI_MenuBase::HandleClickInternal(int idx)
{
    if (onClickHandler) {
        onClickHandler(idx);
    }
}

void UI_MenuBase::Show(const Vector2 origoCM_)
{
    transitionStartTime = GetTime();
    animState = UI_ANIM_STATE::UITEM_EXPANDING;
    origoCM = origoCM_;
    std::cout << "Show" << std::endl;
}

void UI_MenuBase::Hide()
{
    transitionStartTime = GetTime();
    animState = UI_ANIM_STATE::UITEM_CLOSING;
    std::cout << "Hide" << std::endl;
}

bool UI_MenuBase::IsVisible()
{
    return animState != UI_ANIM_STATE::UITEM_CLOSED;
}

void UI_MenuBase::HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx)
{
    if (UI_ANIM_STATE::UITEM_CLOSED == animState) return;
    
    if (UI_ANIM_STATE::UITEM_EXPANDING == animState)
    {
        expansionProgress = Lerp(0.1, 1, Clamp(GetTime() - transitionStartTime, 0, 0.25) / .25f);
        if (FloatEquals(expansionProgress,1.0f))
        {
            animState = UI_ANIM_STATE::UITEM_OPEN;
        }
    }

    if (UI_ANIM_STATE::UITEM_CLOSING == animState)
    {
        expansionProgress = Lerp(1, 0.1, Clamp(GetTime() - transitionStartTime, 0, 0.25) / .25f);
        if (FloatEquals(expansionProgress,0.1f))
        {
            animState = UI_ANIM_STATE::UITEM_CLOSED;
        }
    }
}