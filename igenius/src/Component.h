#ifndef IG_IG_COMPONENTS_H
#define IG_IG_COMPONENTS_H

#include <raylib.h>
#include <vector>
#include <string>
#include <set>
#include "IG_types.h"
#include <cstdint>

typedef enum : std::int32_t
{
    // read from a folder, but inputs defined from code, not ui, behaves like a simple camera (start / stop)
    UNDEFINED = 0,
    VIRTUAL_CAMERA = 1,
    SP_CONVOLUTION = 2,
    IMG_IMG_IMG_OPERATION = 3,
} CMPNAMES;

struct IOSpec
{
   IGDataTypes type;
   bool required; 
};
class Component 
{
public:
    unsigned char id;

    // I am using a string because I have a vague idea
    // that it makes it more extensible to add new component types in the future
    // without relying on enums etc.
    std::string componentType;

    // Datatype in input, and wheter or not a data field is optional
    std::vector<IOSpec> inputTypes;
    IGDataTypes outputType;
};

class UI_Component 
{
public:
    UI_Component(unsigned char id, Rectangle anchor_ );
    const unsigned char c_id;
    Rectangle anchor;


};



#endif