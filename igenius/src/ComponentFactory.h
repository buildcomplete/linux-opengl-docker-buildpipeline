#ifndef IG_COMPONENT_FACTORY_H
#define IG_COMPONENT_FACTORY_H
#include "components/Component.h"
#include "components/SP_CONVOLUTIONUI.h"
#include "ComponentBluePrint.h"
#include <memory>



class ComponentFactory
{
public:
    static ComponentBluePrint GetBluePrint(CMPNAMES name);
    //static UI_Component CreateUI_Component(CMPNAMES name, CellPosition anchor);
    static std::unique_ptr<UI_Component> CreateUI_Component(ComponentBluePrint blueprint, std::uint8_t id, CellPosition anchor);

private:
    static const std::vector<ComponentBluePrint> componentBluePrints;
};

#endif