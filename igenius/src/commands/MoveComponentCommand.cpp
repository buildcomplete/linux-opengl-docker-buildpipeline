#include "MoveComponentCommand.h"

MoveComponentCommand::MoveComponentCommand(Canvas &_canvas, std::uint8_t _componentId, CellPosition _newPosition) :
    canvas(_canvas), componentId(_componentId), newPosition(_newPosition)
{
    originalPosition = canvas.GetComponent(componentId)->anchor;
}

bool MoveComponentCommand::Execute()
{
    return canvas.MoveComponentTo(componentId, newPosition);
}

bool MoveComponentCommand::Undo()
{
    return canvas.MoveComponentTo(componentId, originalPosition);
}
