#pragma once
#include <memory>
class Canvas;

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual bool Execute(Canvas &c) = 0;
    virtual bool Undo(Canvas &c) = 0;
};