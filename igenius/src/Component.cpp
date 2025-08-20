#include "Component.h"

void UI_Component::Draw(const RenderContext& rc) const
{
    const float padding_cm = 0.1f;
    int xp = rc.ch.CmToPixel(anchor.x + padding_cm);
    int yp = rc.ch.CmToPixel(anchor.y + padding_cm);
    int wp = rc.ch.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    int hp = rc.ch.CmToPixel(bluePrint.Height - 2.0f * padding_cm);
    DrawRectangle(xp+1, yp+1, wp-2, hp - 2, PINK);
}

void UI_Components::VirtualCameraUI::Draw(const RenderContext& rc) const
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
