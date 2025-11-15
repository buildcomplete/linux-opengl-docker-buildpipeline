#pragma once
#include <memory>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual bool Execute() = 0;
    virtual bool Undo() = 0;
};