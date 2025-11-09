#include "AddNetworkSegmentCommand.h"

AddNetworkSegmentCommand::AddNetworkSegmentCommand(Canvas& _canvas, std::vector<CellPosition> _anchors) : 
    canvas(_canvas),  anchors(_anchors)
{ }

void AddNetworkSegmentCommand::Execute()
{
    canvas.AddNetworkSegment(anchors, 0);
}

void AddNetworkSegmentCommand::Undo()
{
}
