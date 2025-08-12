#ifndef ENGINE_INPUTEVENTMANAGER_H
#define ENGINE_EVENTMANAGER_H

#include <vector>
#include <unordered_map>
#include <functional>

typedef enum
{
    IG_MOUSE_ZERO                   = 0,
    IG_MOUSE_SELECTING 			    = 1 << 0, //1
    IG_MOUSE_SCREEN_DRAGGING 		= 1 << 1, // 2
    IG_MOUSE_MODE_NETWORK			= 1 << 2, // 4
    IG_MOUSE_DRAW_NETWORK			= 1 << 3, // 8
    IG_MOUSE_POSITION_COMPONENT 	= 1 << 4,
} MOUSE_MODE_FLAGS;

// Raises all enter and exit events to listeners
class Engine_InputEventManager
{
public:
    MOUSE_MODE_FLAGS GetFlags();
    void UpdateStateFromInput();

    bool did_enter_state(MOUSE_MODE_FLAGS testState);
    bool did_exit_state(MOUSE_MODE_FLAGS testState);

private:
    MOUSE_MODE_FLAGS flags = IG_MOUSE_SELECTING;
    MOUSE_MODE_FLAGS flippedFlags = IG_MOUSE_ZERO;
};


#endif