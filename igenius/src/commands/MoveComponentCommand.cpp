#include "MoveComponentCommand.h"

MoveComponentCommand::MoveComponentCommand(Canvas &_canvas, std::uint8_t _componentId, CellPosition _newPosition) :
    canvas(_canvas), componentId(_componentId), newPosition(_newPosition)
{
    originalPosition = canvas.GetComponent(componentId)->anchor;
}

void MoveComponentCommand::Execute()
{
    canvas.MoveComponentTo(componentId, newPosition);
}

void MoveComponentCommand::Undo()
{
    canvas.MoveComponentTo(componentId, originalPosition);
}
