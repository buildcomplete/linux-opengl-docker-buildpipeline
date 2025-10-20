#include "StateContext.h"

bool StateContext::IsInState(INPUT_STATE_FLAGS testState) const
{
    return testState & flags;
}

bool StateContext::DidEnterState(INPUT_STATE_FLAGS testState) const
{
    return (testState & flags & flippedFlags);
}

bool StateContext::DidExitState(INPUT_STATE_FLAGS testState) const
{
    return (testState & ~flags & flippedFlags);
}
