#ifndef STATECONTEXT_H
#define STATECONTEXT_H
#include "../IG_types.h"
#include "raylib.h"
struct StateContext
{
    INPUT_STATE_FLAGS flags;
    INPUT_STATE_FLAGS flippedFlags;
    Vector2 inputCursorDelta;

    bool IsInState(INPUT_STATE_FLAGS testState) const;
    bool DidEnterState(INPUT_STATE_FLAGS testState) const;
    bool DidExitState(INPUT_STATE_FLAGS testState) const;
};
#endif