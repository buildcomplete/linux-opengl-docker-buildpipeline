#include "Component.h"
#include <iostream>
#include <algorithm>


UI_Component::UI_Component(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
    : id(id_), anchor(anchor_), bluePrint(bluePrint_) 
{
}

void UI_Component::Draw(const RenderContext &rc) const
{
    const float padding_cm = 0.1f;
    int xp = rc.ch.CmToPixel(anchor.x + padding_cm);
    int yp = rc.ch.CmToPixel(anchor.y + padding_cm);
    int wp = rc.ch.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    int hp = rc.ch.CmToPixel(bluePrint.Height - 2.0f * padding_cm);
    DrawRectangle(xp+1, yp+1, wp-2, hp - 2, PINK);

    // Draw sockets on the edge as circles
    // In theory the sockets could be on any locations, but for simplicity we add them here
    // The socket shape depends on the type
    // The socket color changes with socket number
    
    // Input sockets on the left,
    if (bluePrint.inputDataTypes.size() != 0)
    {
        int ys0 = rc.ch.CmToPixel(anchor.y);
        float inPosX = xp;
        for (int i=0;i<bluePrint.inputDataTypes.size();++i)
        {
            
            float inPosY = ys0 + rc.ch.pixPr_cm/2.0f + ((float)i * 2) * rc.ch.pixPr_cm ;
            DrawCircleLinesV({inPosX, inPosY}, rc.ch.pixPr_cm/8.0f, WHITE);
        }
    }

    // output on the right
    if (bluePrint.outputDataType.type != DT_NONE)
    {
        float y_out = rc.ch.CmToPixel((float)anchor.y + (float)bluePrint.Height/2.0f) ;
        float outPosX = xp+wp;
        DrawCircleLinesV({outPosX, y_out}, rc.ch.pixPr_cm/8.0f, WHITE);
    }

    
}

UI_Components::VirtualCameraUI::VirtualCameraUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
     : UI_Component(id_, anchor_, bluePrint_) {}

void UI_Components::VirtualCameraUI::Draw(const RenderContext &rc) const
{
    // Depending in state, we should draw either a start or stop button
    float x0 = rc.ch.CmToPixel(anchor.x);
    float y0 = rc.ch.CmToPixel(anchor.y);
    float wp = rc.ch.CmToPixel(bluePrint.Width);
    float hp = rc.ch.CmToPixel(bluePrint.Height);
    //DrawTexture(cameraTexture, 200, 300, WHITE);
    DrawTexturePro(rc.cameraTexture, 
        {(float)0,(float)0,(float)rc.cameraTexture.width, (float)rc.cameraTexture.height},  
        {x0, y0, wp, hp},
        {0, 0},
        0,
        WHITE);
}

UI_Components::SP_CONVOLUTIONUI::SP_CONVOLUTIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
: UI_Component(id_, anchor_, bluePrint_) {}

void UI_Components::SP_CONVOLUTIONUI::Draw(const RenderContext &rc) const
{
   const float padding_cm = 0.1f;
    int xp = rc.ch.CmToPixel(anchor.x + padding_cm);
    int yp = rc.ch.CmToPixel(anchor.y+0.5f);
    int wp = rc.ch.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    int hp = rc.ch.CmToPixel(2);
    DrawRectangleLines(xp + 1, yp + 1, wp - 2, hp - 2, ELECTRIC_BLUE);

    // Draw sockets on the edge as circles
    // In theory the sockets could be on any locations, but for simplicity we add them here
    // The socket shape depends on the type
    // The socket color changes with socket number
    
    // Input sockets on the left,
    float ys0 = yp + hp/2.0f;
    float inPosX = xp;
    DrawCircleLinesV({inPosX, ys0}, rc.ch.pixPr_cm/8.0f, WHITE);
    
    // output on the right
    if (bluePrint.outputDataType.type != DT_NONE)
    {
        float y_out = rc.ch.CmToPixel((float)anchor.y + (float)bluePrint.Height/2.0f) ;
        float outPosX = xp+wp;
        DrawCircleLinesV({outPosX, y_out}, rc.ch.pixPr_cm/8.0f, WHITE);
    } 
}
