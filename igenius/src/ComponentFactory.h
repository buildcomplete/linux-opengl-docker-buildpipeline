#ifndef IG_COMPONENT_FACTORY_H
#define IG_COMPONENT_FACTORY_H
#include "Component.h"



struct ComponentBluePrint
{
    CMPNAMES Name;
    std::uint8_t Width;
    std::uint8_t Heigh;
    std::vector<IOSpec> inputDataTypes;
    IOSpec outputDataType;

    // Constructor to initialize all member variables
    ComponentBluePrint(CMPNAMES name, std::uint8_t width, std::uint8_t height, 
                       std::initializer_list<IOSpec> inputTypes, IOSpec outputTypes)
        : Name(name), Width(width), Heigh(height), 
          inputDataTypes(inputTypes), outputDataType(outputTypes) {}
};

class ComponentFactory
{
public:
    static ComponentBluePrint GetBluePrint(CMPNAMES name);

private:
    static const std::vector<ComponentBluePrint> componentBluePrints;
};

#endif