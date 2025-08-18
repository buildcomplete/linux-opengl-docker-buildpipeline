#ifndef IG_COMPONENT_FACTORY_H
#define IG_COMPONENT_FACTORY_H
#include "Component.h"
#include "ComponentBluePrint.h"



class ComponentFactory
{
public:
    static ComponentBluePrint GetBluePrint(CMPNAMES name);

private:
    static const std::vector<ComponentBluePrint> componentBluePrints;
};

#endif