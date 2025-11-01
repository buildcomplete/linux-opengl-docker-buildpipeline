#ifndef IG_COMPONENT_BLUEPRINT_H
#define IG_COMPONENT_BLUEPRINT_H
#include <cstdint>
#include <vector>
#include <IG_types.h>

typedef enum : std::int32_t
{
    UNDEFINED = 0,
    VIRTUAL_CAMERA = 1, // read from a folder, but inputs defined from code, not ui, behaves like a simple camera (start / stop)
    SP_CONVOLUTION = 2,
    IMG_IMG_IMG_OPERATION = 3,
    DRAWING_COMPONENT = 4, // Special component to make drawings to use on the components as strokes
} CMPNAMES;

struct IOSpec
{
    IGDataTypes type;
    std::uint8_t row;
};

// struct IOPlacement
// {
//     IOSpec spec;
//     CellPosition anchor; // Actual placement on component, allowing to move pins
// };


struct ComponentBluePrint
{
    CMPNAMES Name;
    std::uint8_t Width;
    std::uint8_t Height;
    std::vector<IOSpec> inputDataTypes;
    IOSpec outputDataType;

    ComponentBluePrint()
        : Name(CMPNAMES::UNDEFINED), Width(1), Height(1),
          inputDataTypes({{IGDataTypes::DT_NONE,0}}), 
          outputDataType({IGDataTypes::DT_NONE,0}) {}

    // Constructor to initialize all member variables
    ComponentBluePrint(CMPNAMES name, std::uint8_t width, std::uint8_t height,
                       std::initializer_list<IOSpec> inputTypes, IOSpec outputTypes)
        : Name(name), Width(width), Height(height),
          inputDataTypes(inputTypes), 
          outputDataType(outputTypes) {}

    // Copy constructor
    ComponentBluePrint(const ComponentBluePrint &other)
        : Name(other.Name), Width(other.Width), Height(other.Height),
          inputDataTypes(other.inputDataTypes),
          outputDataType(other.outputDataType) {}

    // Copy assignment operator
    ComponentBluePrint &operator=(const ComponentBluePrint &other)
    {
        if (this != &other)
        { // Self-assignment check
            Name = other.Name;
            Width = other.Width;
            Height = other.Height;
            inputDataTypes = other.inputDataTypes;
            outputDataType = other.outputDataType;
        }
        return *this;
    }
};
#endif