#include "Engine_StateManager.h"
#include <raylib.h>

StateContext Engine_StateManager::HandleEvents()
{
    INPUT_STATE_FLAGS targetState = IG_INPUT_ZERO;
    targetState = (INPUT_STATE_FLAGS)(targetState | ((IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_LEFT_CONTROL)) ? IG_INPUT_SCREEN_DRAGGING : IG_INPUT_ZERO));
    targetState = (INPUT_STATE_FLAGS)(targetState | ((IsKeyDown(KEY_LEFT_SHIFT)) ? IG_INPUT_MODE_DRAW_NETWORK : IG_INPUT_ZERO));
    targetState = (INPUT_STATE_FLAGS)(targetState | ((IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? IG_INPUT_TRY_COMMAND : IG_INPUT_ZERO));
    
    // Set MOUSE SELECTING if we are not positioning components or in network mode
    targetState = (INPUT_STATE_FLAGS)(targetState | ((targetState & (IG_INPUT_MODE_DRAW_NETWORK | IG_INPUT_MODE_PLACE_COMPONENT | IG_INPUT_MODE_MOVE_COMPONENT ) ) ? IG_INPUT_ZERO : IG_INPUT_MODE_SELECTING));
    state.flippedFlags = (INPUT_STATE_FLAGS)(targetState ^ state.flags);
    state.flags = targetState;
    state.inputCursorDelta = GetMouseDelta();
    return state;
}

void Engine_StateManager::SetFocusComponent(UI_Component *c)
{
    focusedComponent = c;
}

UI_Component* Engine_StateManager::GetFocusComponent()
{
    return focusedComponent;
}
