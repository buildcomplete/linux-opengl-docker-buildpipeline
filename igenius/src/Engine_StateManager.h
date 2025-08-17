#ifndef ENGINE_STATEMANAGER_H
#define ENGINE_STATEMANAGER_H

typedef enum
{
    IG_MOUSE_ZERO                   = 0,
    IG_MOUSE_SELECTING 			    = 1 << 0, //1
    IG_MOUSE_SCREEN_DRAGGING 		= 1 << 1, // 2
    IG_MOUSE_MODE_NETWORK			= 1 << 2, // 4
    // Try to execute command from mouse cursor depending on state...
    IG_MOUSE_TRY_COMMAND			= 1 << 3, // 8
    IG_MOUSE_POSITION_COMPONENT 	= 1 << 4, // 16
    IG_MOUSE_DELETE                 = 1 << 5, // 32

} MOUSE_MODE_FLAGS;

/**
 * Interprets key and mouse events to manage game states and transitions, 
 * facilitating communication with the Canvas for item state changes.
 */
class Engine_StateManager
{
public:
    MOUSE_MODE_FLAGS GetFlags() const;
    void UpdateFromInput();

    bool DidEnterState(MOUSE_MODE_FLAGS testState) const;
    bool DidExitState(MOUSE_MODE_FLAGS testState) const;

private:
    MOUSE_MODE_FLAGS flags = IG_MOUSE_SELECTING;
    MOUSE_MODE_FLAGS flippedFlags = IG_MOUSE_ZERO;
};


#endif