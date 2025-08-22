#include "Engine_StateManager.h"
#include <raylib.h>

StateContext Engine_StateManager::UpdateFromInput()
{
    MOUSE_MODE_FLAGS targetState = IG_MOUSE_ZERO;
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_SPACE)) ? IG_MOUSE_SCREEN_DRAGGING : IG_MOUSE_ZERO));
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((IsKeyDown(KEY_LEFT_SHIFT)) ? IG_MOUSE_MODE_NETWORK : IG_MOUSE_ZERO));
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? IG_MOUSE_TRY_COMMAND : IG_MOUSE_ZERO));
    
    // Set MOUSE SELECTING if we are not positioning components or in network mode
    targetState = (MOUSE_MODE_FLAGS)(targetState | ((targetState & (IG_MOUSE_MODE_NETWORK | IG_MOUSE_POSITION_COMPONENT | IG_MOUSE_POSITION_COMPONENT ) ) ? IG_MOUSE_ZERO : IG_MOUSE_SELECTING));
    state.flippedFlags = (MOUSE_MODE_FLAGS)(targetState ^ state.flags);
    state.flags = targetState;
    return state;
}

