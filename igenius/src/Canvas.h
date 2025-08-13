#ifndef IG_CANVAS_H
#define IG_CANVAS_H
#include <vector>
#include "Component.h"
#include "CoordinateHelper.h"

// Represents a single canvas with components and networks
class Canvas
{
public:
    Canvas();

    void Update();

    void Draw(CoordinateHelper&);

    std::vector<UI_Component> components;
};

#endif