#include "AddNetworkSegmentCommand.h"
#include "canvas/Canvas.h"

AddNetworkSegmentCommand::AddNetworkSegmentCommand(std::vector<CellPosition> _anchors) : 
     anchors(_anchors)
{ }

bool AddNetworkSegmentCommand::Execute(Canvas &c)
{
    segmentInsertionId=c.AddNetworkSegment(anchors, 0);
    return segmentInsertionId != 0;
}

bool AddNetworkSegmentCommand::Undo(Canvas &c)
{
    return c.RemoveNetworkSegment(segmentInsertionId);
}
