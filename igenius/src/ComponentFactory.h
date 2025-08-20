#ifndef IG_COMPONENT_FACTORY_H
#define IG_COMPONENT_FACTORY_H
#include "Component.h"
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