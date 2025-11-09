#include "commands/CommandStack.h"

void CommandStack::Execute(std::unique_ptr<ICommand> command) {
    command->Execute();
    undoStack.push_back(std::move(command));
    redoStack.clear();
}

void CommandStack::Undo() {
    if (!undoStack.empty()) {
        undoStack.back()->Undo();
        redoStack.push_back(std::move(undoStack.back()));
        undoStack.pop_back();
    }
}

void CommandStack::Redo() {
    if (!redoStack.empty()) {
        redoStack.back()->Execute();
        undoStack.push_back(std::move(redoStack.back()));
        redoStack.pop_back();
    }
}