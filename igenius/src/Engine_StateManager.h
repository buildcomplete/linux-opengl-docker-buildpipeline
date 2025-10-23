#ifndef ENGINE_STATEMANAGER_H
#define ENGINE_STATEMANAGER_H
#include "IG_types.h"
#include "context/StateContext.h"
#include "components/Component.h"
#include "components/UI_RadialInputSelectionMenu.h"

/**
 * Interprets key and mouse events to manage game states and transitions, 
 * facilitating communication with the Canvas for item state changes.
 */
class Engine_StateManager
{
public:
    Engine_StateManager();
    
    StateContext HandleEvents();
    void SetFocusComponent(UI_Component* c);
    UI_Component *GetFocusComponent() const;
    void UpdateMenus(const NavigationContext& navCtx);
    void Draw(const RenderContext rc, const NavigationContext navCtx) const;

private:
   StateContext state;
   UI_Component* focusedComponent = nullptr;
   UI_RadialInputSelectionMenu inputSelectionMenu;
   void OnMenuSelectInputModeClick(int segmentIndex);
};


#endif