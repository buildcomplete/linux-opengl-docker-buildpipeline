#include "StateContext.h"

bool StateContext::IsInState(MOUSE_MODE_FLAGS testState) const
{
    return testState & flags;
}

bool StateContext::DidEnterState(MOUSE_MODE_FLAGS testState) const
{
    return (testState & flags & flippedFlags);
}

bool StateContext::DidExitState(MOUSE_MODE_FLAGS testState) const
{
    return (testState & ~flags & flippedFlags);
}
