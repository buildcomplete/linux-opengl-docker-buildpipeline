#include "ComponentFactory.h"
#include <cstdint>
#include <vector>
// Initialize the static member outside the class definition
const std::vector<ComponentBluePrint> ComponentFactory::componentBluePrints = {
    ComponentBluePrint(VIRTUAL_CAMERA, 1, 1, {}, {DT_IMAGE, false}),
    ComponentBluePrint(SP_CONVOLUTION, 3, 2, {{DT_IMAGE, true}}, {DT_IMAGE, true}),
    ComponentBluePrint(IMG_IMG_IMG_OPERATION, 2, 3, {{DT_IMAGE, true}, {DT_IMAGE, true}}, {DT_IMAGE, true}),
};

ComponentBluePrint ComponentFactory::GetBluePrint(CMPNAMES name)
{
    for (const auto& blueprint : componentBluePrints)
    {
        if (blueprint.Name == name)
            return blueprint;
    }
    return ComponentBluePrint(UNDEFINED, 1, 1, {}, {DT_NONE, false});
}

std::unique_ptr<UI_Component> ComponentFactory::CreateUI_Component(ComponentBluePrint blueprint, std::uint8_t id, CellPosition anchor)
{
    switch (blueprint.Name)
    {
    case CMPNAMES::VIRTUAL_CAMERA:
        return std::make_unique<UI_Components::VirtualCameraUI>(id, anchor, blueprint);
    
    default:
        return std::make_unique<UI_Component>(id, anchor, blueprint );
    }
}
