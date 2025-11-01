#include "Engine_StateManager.h"
#include <raylib.h>
#include <bitset>

Engine_StateManager::Engine_StateManager() :
    inputSelectionMenu([this](int idx) { OnMenuSelectInputModeClick(idx); })
{};



 INPUT_STATE_FLAGS inputModeFlag = INPUT_STATE_FLAGS::IG_INPUT_MODE_SELECTING;
void Engine_StateManager::OnMenuSelectInputModeClick(int segmentIndex)
{
    std::cout << "** void Engine_StateManager::OnMenuSelectOperationClick(int segmentIndex= " << segmentIndex << ")" << std::endl;
    //{"SELECT", "NET", "PLACE", "MOVE", "DELETE"};
    const INPUT_STATE_FLAGS indexMap[]= 
    {
        INPUT_STATE_FLAGS::IG_INPUT_MODE_SELECTING,
        INPUT_STATE_FLAGS::IG_INPUT_MODE_DRAW_NETWORK,
        INPUT_STATE_FLAGS::IG_INPUT_MODE_PLACE_COMPONENT,
        INPUT_STATE_FLAGS::IG_INPUT_MODE_MOVE_COMPONENT,
        INPUT_STATE_FLAGS::IG_INPUT_MODE_DELETE,
    };
    inputModeFlag = indexMap[segmentIndex];
    inputSelectionMenu.Hide();
}


StateContext Engine_StateManager::HandleEvents()
{
    INPUT_STATE_FLAGS targetState = inputModeFlag;
    targetState = (INPUT_STATE_FLAGS)(targetState | ((IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_LEFT_CONTROL)) ? IG_INPUT_SCREEN_DRAGGING : IG_INPUT_ZERO));
    targetState = (INPUT_STATE_FLAGS)(targetState | ((IsKeyDown(KEY_LEFT_SHIFT) || inputSelectionMenu.IsVisible()) ? IG_INPUT_SELECT_MODE : IG_INPUT_ZERO));
    //targetState = (INPUT_STATE_FLAGS)(targetState | ((IsKeyDown(KEY_LEFT_SHIFT)) ? IG_INPUT_MODE_DRAW_NETWORK : IG_INPUT_ZERO));
    targetState = (INPUT_STATE_FLAGS)(targetState | ((IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? IG_INPUT_TRY_COMMAND : IG_INPUT_ZERO));

   
    
    // Set MOUSE SELECTING if we are not positioning components or in network mode
    state.flippedFlags = (INPUT_STATE_FLAGS)(targetState ^ state.flags);
    state.flags = targetState;
    state.inputCursorDelta = GetMouseDelta();
    if (state.flippedFlags )
    {
        std::bitset<32> binary(state.flippedFlags ); // converts the integer to 32-bit binary

        std::cout << "Flipped flags:" << binary << std::endl;
    }

    if (state.DidEnterState(IG_INPUT_SELECT_MODE))
    {
        inputModeFlag = IG_INPUT_ZERO;// While selecting, reset mouse selecting mode until new selection made
    }


    return state;
}

void Engine_StateManager::SetFocusComponent(CanvasComponentBase *c)
{
    focusedComponent = c;
}

CanvasComponentBase* Engine_StateManager::GetFocusComponent() const
{
    return focusedComponent;
}

void Engine_StateManager::UpdateMenus(const NavigationContext& navCtx)
{       
    if (state.DidEnterState(IG_INPUT_SELECT_MODE) 
        && false == inputSelectionMenu.IsVisible())
    {
        inputSelectionMenu.Show(navCtx.mousePosWorldCm);
    }

    if (inputSelectionMenu.IsVisible())
    {
        inputSelectionMenu.HandleEventsAndTime(state, navCtx);
    }

    if (focusedComponent != nullptr)
    {
        bool keepFocus = focusedComponent->HandleEventsWhileFocused(state, navCtx);
        if (!keepFocus)
        {
            SetFocusComponent(nullptr);
        }
    }
}

void Engine_StateManager::Draw(const RenderContext rc, const NavigationContext navCtx) const
{
    if (focusedComponent != nullptr)
    {
        focusedComponent->FocusedDraw(rc, navCtx);
    }

    inputSelectionMenu.Draw(rc, navCtx);
}
