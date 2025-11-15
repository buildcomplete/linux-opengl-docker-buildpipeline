#include "AddNetworkSegmentCommand.h"

AddNetworkSegmentCommand::AddNetworkSegmentCommand(Canvas& _canvas, std::vector<CellPosition> _anchors) : 
    canvas(_canvas),  anchors(_anchors)
{ }

bool AddNetworkSegmentCommand::Execute()
{
    segmentInsertionId=canvas.AddNetworkSegment(anchors, 0);
    return segmentInsertionId != 0;
}

bool AddNetworkSegmentCommand::Undo()
{
    return canvas.RemoveNetworkSegment(segmentInsertionId);
}
