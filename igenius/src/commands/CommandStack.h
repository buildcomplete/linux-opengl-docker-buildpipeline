#pragma once

#include <vector>
#include <memory>
#include "commands/ICommand.h"


class CommandStack {
public:
    CommandStack(Canvas &c);
    void Execute(std::unique_ptr<ICommand> command);
    void Undo();
    void Redo();

private:
    std::vector<std::unique_ptr<ICommand>> undoStack;
    std::vector<std::unique_ptr<ICommand>> redoStack;
    Canvas &canvas;
};