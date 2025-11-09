#pragma once
#include "context/NavigationContext.h"
#include "context/RenderContext.h"
#include "context/StateContext.h"
#include "canvas/Canvas.h"
#include "commands/ICommand.h"

class CanvasToolBase
{
public:
    CanvasToolBase(Canvas &c);
    virtual void Draw(const RenderContext &rc, const NavigationContext &navCtx) const = 0;
    virtual std::unique_ptr<ICommand> HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx)=0;

protected:
    Canvas &canvas;
};