#pragma once
#include "commands/ICommand.h"
#include <vector>
#include "IG_types.h"
#include "canvas/Canvas.h"

class AddNetworkSegmentCommand : public ICommand
{

public:
    AddNetworkSegmentCommand(Canvas &_canvas, std::vector<CellPosition> _anchors);
    void Execute() override;
    void Undo() override;

private:
    std::vector<CellPosition> anchors;
    Canvas &canvas;
    std::uint32_t segmentInsertionId;
};