#pragma once
#include "canvas/components/CanvasComponents.h"
#include "canvas/components/SP_CONVOLUTIONUI.h"
#include "canvas/components/ComponentBluePrint.h"
#include <memory>



class ComponentFactory
{
public:
    static ComponentBluePrint GetBluePrint(CMPNAMES name);
    //static UI_Component CreateUI_Component(CMPNAMES name, CellPosition anchor);
    static std::unique_ptr<CanvasComponentBase> CreateUI_Component(ComponentBluePrint blueprint, std::uint8_t id, CellPosition anchor);

private:
    static const std::vector<ComponentBluePrint> componentBluePrints;
};
