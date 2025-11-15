#pragma once
#include "commands/ICommand.h"
#include <vector>
#include "IG_types.h"
#include "canvas/Canvas.h"

class MoveComponentCommand : public ICommand
{
public:
    MoveComponentCommand(std::uint8_t _componentId, CellPosition _newPosition);
    bool Execute(Canvas &c) override;
    bool Undo(Canvas &c) override;

private:
    std::uint8_t componentId;
    CellPosition newPosition;
    CellPosition originalPosition;
};