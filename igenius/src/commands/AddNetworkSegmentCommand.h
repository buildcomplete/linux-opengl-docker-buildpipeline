#pragma once
#include "commands/ICommand.h"
#include <vector>
#include "IG_types.h"

class AddNetworkSegmentCommand : public ICommand
{

public:
    AddNetworkSegmentCommand(std::vector<CellPosition> _anchors);
    bool Execute(Canvas &c) override;
    bool Undo(Canvas &c) override;

private:
    std::vector<CellPosition> anchors;
    std::uint32_t segmentInsertionId;
};