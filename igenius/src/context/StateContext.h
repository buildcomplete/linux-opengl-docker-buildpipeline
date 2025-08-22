#ifndef STATECONTEXT_H
#define STATECONTEXT_H
#include "../IG_types.h"

struct StateContext
{
    MOUSE_MODE_FLAGS flags;
    MOUSE_MODE_FLAGS flippedFlags;

    bool IsInState(MOUSE_MODE_FLAGS testState) const;
    bool DidEnterState(MOUSE_MODE_FLAGS testState) const;
    bool DidExitState(MOUSE_MODE_FLAGS testState) const;
};
#endif