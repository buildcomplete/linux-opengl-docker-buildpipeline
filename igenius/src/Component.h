#ifndef IG_IG_COMPONENTS_H
#define IG_IG_COMPONENTS_H

#include <raylib.h>
#include <vector>
#include <string>
#include <set>
#include "IG_types.h"

// read from a folder, but inputs defined from code, not ui, behaves like a simple camera (start / stop)
namespace CMPNAME
{
    const std::string VIRTUAL_CAMERA = "VIRTUAL CAMERA";
    const std::string SP_CONVOLUTION = "CONVOLUTION";
    const std::string IMG_IMG_IMG_OPERATION = "Image Image => Image operation";
}

class Component 
{
public:
    char id;

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
    UI_Component(char id, Rectangle anchor_ );
    const char c_id;
    Rectangle anchor;
    std::vector<Vector2> inputAnchors;
    Vector2 outputAnchor;
};

// Factory should be able to made pr depth / level 
// as we limit number of cmoponents to each plan to 255
class UI_PlaneComponentManager
{
private:
    std::set<char> avaliableKeys;
    std::vector<UI_Component> uiComponents;

public:
    UI_PlaneComponentManager()
    {
        char i=0;
        do
        {
            i++;
            avaliableKeys.insert(i);
            /* code */
        } while (i!=255);
    }

    UI_Component Create(const std::string name, const Vector2 anchor)
    {
        if (CMPNAME::VIRTUAL_CAMERA == name)
        {

        }
        return UI_Component(0, {0.0,0.0,0.0,0.0});
    }
};

#endif