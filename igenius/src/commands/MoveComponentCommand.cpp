#include "MoveComponentCommand.h"

MoveComponentCommand::MoveComponentCommand(std::uint8_t _componentId, CellPosition _newPosition) :
    componentId(_componentId), newPosition(_newPosition)
{
}

bool MoveComponentCommand::Execute(Canvas &c )
{
    originalPosition = c.GetComponent(componentId)->anchor;
    return c.MoveComponentTo(componentId, newPosition);
}

bool MoveComponentCommand::Undo(Canvas &c)
{
    return c.MoveComponentTo(componentId, originalPosition);
}
