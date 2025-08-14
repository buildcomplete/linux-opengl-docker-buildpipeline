#ifndef IG_IG_COMPONENTS_H
#define IG_IG_COMPONENTS_H

#include <raylib.h>
#include <vector>
#include <string>
#include <set>
#include "IG_types.h"

class Component 
{
public:
    unsigned char id;

    // I am using a string because I have a vague idea
    // that it makes it more extensible to add new component types in the future
    // without relying on enums etc.
    std::string componentType;

    // Datatype in input, and wheter or not a data field is optional
    std::vector<std::pair<IGDataTypes, bool>> inputTypes;
    IGDataTypes outputType;
};

class UI_Component 
{
public:
    UI_Component(unsigned char id, Rectangle anchor_ );
    const unsigned char c_id;
    Rectangle anchor;
    std::vector<Vector2> inputAnchors;
    Vector2 outputAnchor;

    bool Inside(Vector2 pWorld);
};

#endif