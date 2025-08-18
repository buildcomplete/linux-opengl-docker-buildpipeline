#ifndef IG_IG_COMPONENTS_H
#define IG_IG_COMPONENTS_H

#include <raylib.h>
#include <vector>
#include <string>
#include <set>
#include "IG_types.h"
#include <cstdint>
#include "ComponentBluePrint.h"


// class Component 
// {
// public:
//     unsigned char id = 0;

//     // I am using a string because I have a vague idea
//     // that it makes it more extensible to add new component types in the future
//     // without relying on enums etc.
//     std::string componentType;

//     // Datatype in input, and wheter or not a data field is optional
//     std::vector<IOSpec> inputTypes;
//     IGDataTypes outputType;
// };

struct UI_Component 
{
    std::uint8_t c_id = 0;
    CellPosition anchor = {0,0};
    ComponentBluePrint bluePrint;
    // UI_Component(std::uint8_t c_id_, CellPosition anchor_, ComponentBluePrint bluePrint_) : c_id(c_id_), anchor(anchor_), bluePrint(bluePrint_) 
    // {}
};



#endif