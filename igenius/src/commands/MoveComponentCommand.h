#pragma once
#include "commands/ICommand.h"
#include <vector>
#include "IG_types.h"
#include "canvas/Canvas.h"

class MoveComponentCommand : public ICommand
{

public:
    MoveComponentCommand(Canvas &_canvas, std::uint8_t _componentId, CellPosition _newPosition);
    bool Execute() override;
    bool Undo() override;

private:
    Canvas &canvas;
    std::uint8_t componentId;
    CellPosition newPosition;
    CellPosition originalPosition;
};