#ifndef IG_IG_COMPONENTS_H
#define IG_IG_COMPONENTS_H

#include <raylib.h>
#include <vector>
#include <string>
#include <set>
#include "IG_types.h"
#include <cstdint>
#include "ComponentBluePrint.h"
#include "CoordinateHelper.h"


// class Component 
// {
// public:
//     std::uint8_t id = 0;

//     // Datatype in input, and wheter or not a data field is optional
//     std::vector<IOSpec> inputTypes;
//     IGDataTypes outputType;

// };

// lightweight non-owning context
struct RenderContext {
    CoordinateHelper& ch;
    Texture& cameraTexture;
    // // optional helpers/wrappers for your C API:
    // void DrawRectangle(int x, int y, int w, int h, Color c) const {
    //     ::DrawRectangle(x,y,w,h,c);
    // }
    // void DrawTexturePro(const Texture& tex, Rect src, Rect dst, Vec2 origin, float rot, Color tint) const {
    //     ::DrawTexturePro(tex, src, dst, origin, rot, tint);
    // }
};

class UI_Component 
{
public:
    std::uint8_t id = 0;
    CellPosition anchor = {0,0};
    ComponentBluePrint bluePrint;

    UI_Component(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint& bluePrint_ );
    
    virtual void Draw(const RenderContext&) const;
};

namespace UI_Components
{
    class VirtualCameraUI : public UI_Component
    {
    public:
        VirtualCameraUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint& bluePrint_ );
        virtual void Draw(const RenderContext&) const override; 
    };

    class SP_CONVOLUTIONUI : public UI_Component
    {
    public:
        SP_CONVOLUTIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint& bluePrint_ );
        virtual void Draw(const RenderContext&) const override; 
    };
}





#endif