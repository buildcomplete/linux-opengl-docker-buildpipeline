#include "commands/CommandStack.h"
#include <iostream>
#include "CommandStack.h"

CommandStack::CommandStack(Canvas &c) : canvas(c)
{}

void CommandStack::Execute(std::unique_ptr<ICommand> command)
{
    if (command->Execute(canvas))
    {
        undoStack.push_back(std::move(command));
        redoStack.clear();
    }
    else
    {
        std::cerr << "Failed executing command, not adding to stack" << std::endl;
    }
}

void CommandStack::Undo() 
{
    if (!undoStack.empty()) 
    {
        if (!undoStack.back()->Undo(canvas))
        {
            std::cerr << "Failed Undo of command" << std::endl;
        }
        redoStack.push_back(std::move(undoStack.back()));
        undoStack.pop_back();
    }
}

void CommandStack::Redo() 
{
    if (!redoStack.empty()) 
    {
        redoStack.back()->Execute(canvas);
        undoStack.push_back(std::move(redoStack.back()));
        redoStack.pop_back();
    }
}