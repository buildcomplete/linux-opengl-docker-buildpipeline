#pragma once
#include "IG_types.h"
#include "context/StateContext.h"
#include "canvas/components/CanvasComponents.h"
#include "menus/UI_RadialMenuInputSelector.h"

/**
 * Interprets key and mouse events to manage game states and transitions, 
 * facilitating communication with the Canvas for item state changes.
 */
class Engine_StateManager
{
public:
    Engine_StateManager();
    
    StateContext HandleEvents();
    void SetFocusComponent(CanvasComponentBase* c);
    CanvasComponentBase *GetFocusComponent() const;
    void UpdateMenus(const NavigationContext& navCtx);
    void Draw(const RenderContext rc, const NavigationContext navCtx) const;

private:
   StateContext state;
   CanvasComponentBase* focusedComponent = nullptr;
   UI_RadialMenuInputSelector inputSelectionMenu;
   void OnMenuSelectInputModeClick(INPUT_STATE_FLAGS newMode);
};
