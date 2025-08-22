#ifndef ENGINE_STATEMANAGER_H
#define ENGINE_STATEMANAGER_H
#include "IG_types.h"
    #include "context/StateContext.h"


/**
 * Interprets key and mouse events to manage game states and transitions, 
 * facilitating communication with the Canvas for item state changes.
 */
class Engine_StateManager
{
public:
    StateContext UpdateFromInput();
private:
   StateContext state;
};


#endif