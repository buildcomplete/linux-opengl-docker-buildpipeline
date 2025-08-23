#include "Component.h"
#include <iostream>
#include <algorithm>
#include "rshapes.c"

// Mod of DrawCircleSectorLines as I dont wont the cap,
// 
// Draw a piece of a circle outlines
void DrawCircleSelectorLines(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color)
{
    if (startAngle == endAngle) return;
    if (radius <= 0.0f) radius = 0.1f;  // Avoid div by zero issue

    // Function expects (endAngle > startAngle)
    if (endAngle < startAngle)
    {
        // Swap values
        float tmp = startAngle;
        startAngle = endAngle;
        endAngle = tmp;
    }

    int minSegments = (int)ceilf((endAngle - startAngle)/90);

    if (segments < minSegments)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        float th = acosf(2*powf(1 - SMOOTH_CIRCLE_ERROR_RATE/radius, 2) - 1);
        segments = (int)((endAngle - startAngle)*ceilf(2*PI/th)/360);

        if (segments <= 0) segments = minSegments;
    }

    float stepLength = (endAngle - startAngle)/(float)segments;
    float angle = startAngle;
    bool showCapLines=true;
    float opositeAngle = endAngle + 90;
    rlBegin(RL_LINES);
        if (showCapLines)
        {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlVertex2f(center.x + cosf(DEG2RAD*opositeAngle)*radius, center.y + sinf(DEG2RAD*opositeAngle)*radius);
            rlVertex2f(center.x + cosf(DEG2RAD*angle)*radius, center.y + sinf(DEG2RAD*angle)*radius);
        }

        for (int i = 0; i < segments; i++)
        {
            rlColor4ub(color.r, color.g, color.b, color.a);

            rlVertex2f(center.x + cosf(DEG2RAD*angle)*radius, center.y + sinf(DEG2RAD*angle)*radius);
            rlVertex2f(center.x + cosf(DEG2RAD*(angle + stepLength))*radius, center.y + sinf(DEG2RAD*(angle + stepLength))*radius);

            angle += stepLength;
        }

        if (showCapLines)
        {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlVertex2f(center.x + cosf(DEG2RAD*opositeAngle)*radius, center.y + sinf(DEG2RAD*opositeAngle)*radius);
            rlVertex2f(center.x + cosf(DEG2RAD*angle)*radius, center.y + sinf(DEG2RAD*angle)*radius);
        }
    rlEnd();
}


UI_Component::UI_Component(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
    : id(id_), anchor(anchor_), bluePrint(bluePrint_) 
{
}

void UI_Component::Draw(const RenderContext &rc) const
{
    const float padding_cm = 0.1f;
    DrawStandardComponentFrame(rc, padding_cm);

    DrawStandardSockets(rc, padding_cm);
}

bool UI_Component::TryStartCommand(const StateContext &stCtx, const NavigationContext& navCtx, const RenderContext& rndCtx)
{
    Vector2 offset = Vector2Subtract( navCtx.mousePosWorldCm, {(float)anchor.x, (float)anchor.y});
    std::cout << "Clicked component: " << id << " mousePosWorldCm: " << navCtx.mousePosWorldCm.x << ", " << navCtx.mousePosWorldCm.y << std::endl;
    std::cout << "Clicked component: " << id << " anchor: " << anchor.x << ", " << anchor.y << std::endl;
    std::cout << "Clicked component: " << id << " cmp offset: " << offset.x << ", " << offset.y << std::endl;
    return false;
}

void UI_Component::DrawStandardComponentFrame(const RenderContext &rc, const float padding_cm) const
{
    int xp = rc.CmToPixel(anchor.x + padding_cm);
    int yp = rc.CmToPixel(anchor.y + padding_cm);
    int wp = rc.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    int hp = rc.CmToPixel(bluePrint.Height - 2.0f * padding_cm);
    DrawRectangle(xp + 1, yp + 1, wp - 2, hp - 2, PINK);
}

void UI_Component::DrawStandardSockets(const RenderContext &rc, float padding_cm) const
{
    int xp = rc.CmToPixel(anchor.x + padding_cm);
    int wp = rc.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    // Draw sockets on the edge as circles
    // In theory the sockets could be on any locations, but for simplicity we add them here
    // The socket shape depends on the type
    // The socket color changes with socket number
    
    // Input sockets on the left,
    if (bluePrint.inputDataTypes.size() != 0)
    {
        int ys0 = rc.CmToPixel(anchor.y);
        float inPosX = xp;
        for (int i=0;i<bluePrint.inputDataTypes.size();++i)
        {
            
            float inPosY = ys0 + rc.pixPr_cm/2.0f + ((float)i * 2) * rc.pixPr_cm ;
            DrawCircleLinesV({inPosX, inPosY}, rc.pixPr_cm/8.0f, WHITE);
        }
    }

    // output on the right
    if (bluePrint.outputDataType.type != DT_NONE)
    {
        float y_out = rc.CmToPixel((float)anchor.y + (float)bluePrint.Height/2.0f) ;
        float outPosX = xp+wp;
        DrawCircleLinesV({outPosX, y_out}, rc.pixPr_cm/8.0f, WHITE);
    }
}

UI_Components::VirtualCameraUI::VirtualCameraUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
     : UI_Component(id_, anchor_, bluePrint_) {}

void UI_Components::VirtualCameraUI::Draw(const RenderContext &rc) const
{
    // Depending in state, we should draw either a start or stop button
    float x0 = rc.CmToPixel(anchor.x);
    float y0 = rc.CmToPixel(anchor.y);
    float wp = rc.CmToPixel(bluePrint.Width);
    float hp = rc.CmToPixel(bluePrint.Height);
    
    if (isCapturing)
    {
        DrawRectangle(x0, y0, wp,hp, ELECTRIC_BLUE );
    }


    DrawTexturePro(rc.cameraTexture, 
        {(float)0,(float)0,(float)rc.cameraTexture.width, (float)rc.cameraTexture.height},  
        {x0, y0, wp, hp},
        {0, 0},
        0,
        WHITE);

    
    DrawStandardSockets(rc, 0);
}

bool UI_Components::VirtualCameraUI::TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx, const RenderContext &rndCtx)
{
    Vector2 offset = Vector2Subtract( navCtx.mousePosWorldCm, {(float)anchor.x, (float)anchor.y});
    std::cout << "Clicked camera: " << id << " cmp offset: " << offset.x << ", " << offset.y << std::endl;
    isCapturing = !isCapturing;
    return false;
}

UI_Components::SP_CONVOLUTIONUI::SP_CONVOLUTIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
    : UI_Component(id_, anchor_, bluePrint_) {}

UI_Components::IMG_IMG_IMG_OPERATIONUI::IMG_IMG_IMG_OPERATIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
    : UI_Component(id_, anchor_, bluePrint_) {}


void UI_Components::SP_CONVOLUTIONUI::Draw(const RenderContext &rc) const
{
   const float padding_cm = 0.1f;
    int xp = rc.CmToPixel(anchor.x + padding_cm);
    int yp = rc.CmToPixel(anchor.y+0.5f);
    int wp = rc.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    int hp = rc.CmToPixel(2);
    DrawRectangleLines(xp + 1, yp + 1, wp - 2, hp - 2, ELECTRIC_BLUE);

    // Draw sockets on the edge as circles
    // In theory the sockets could be on any locations, but for simplicity we add them here
    // The socket shape depends on the type
    // The socket color changes with socket number
    
    // Input sockets on the left,
    float ys0 = yp + hp/2.0f;
    float inPosX = xp;
    DrawCircleLinesV({inPosX, ys0}, rc.pixPr_cm/8.0f, WHITE);
    
    // output on the right
    if (bluePrint.outputDataType.type != DT_NONE)
    {
        float y_out = rc.CmToPixel((float)anchor.y + (float)bluePrint.Height/2.0f) ;
        float outPosX = xp+wp;
        DrawCircleLinesV({outPosX, y_out}, rc.pixPr_cm/8.0f, WHITE);
    }

    // At the center of the components, draw convolution widget that both show the current signal, 
    // and shows the shape of the current signal in the center
    // We should be able to reach on click on the selector for selecting signal,
    // And to listen for click on direction selectors
    float cx = rc.CmToPixel((float)anchor.x+(float)bluePrint.Width/2.0f);
    float cy = rc.CmToPixel((float)anchor.y + (float)bluePrint.Height/2.0f);
    
    float t = Wrap(GetTime(),0.0f, 4.0f) * 90;
    // rotate every 4 second;

    DrawCircleSelectorLines({cx,cy}, rc.pixPr_cm*0.75f, 90+t, 270+t, 1, WHITE );
}


void UI_Components::IMG_IMG_IMG_OPERATIONUI::Draw(const RenderContext &rc) const
{
    const float padding_cm = 0.1f;
    DrawStandardComponentFrame(rc, padding_cm);

    // Draw sockets on the edge as circles
    // In theory the sockets could be on any locations, but for simplicity we add them here
    // The socket shape depends on the type
    // The socket color changes with socket number
    
    DrawStandardSockets(rc, padding_cm);
    // Input sockets on the left,
    // float ys0 = yp + hp/2.0f;
    // float inPosX = xp;
    // DrawCircleLinesV({inPosX, ys0}, rc.ch.pixPr_cm/8.0f, WHITE);
    
    // // output on the right
    // if (bluePrint.outputDataType.type != DT_NONE)
    // {
    //     float y_out = rc.CmToPixel((float)anchor.y + (float)bluePrint.Height/2.0f) ;
    //     float outPosX = xp+wp;
    //     DrawCircleLinesV({outPosX, y_out}, rc.ch.pixPr_cm/8.0f, WHITE);
    // }

    // At the center of the components, draw convolution widget that both show the current signal, 
    // and shows the shape of the current signal in the center
    // We should be able to reach on click on the selector for selecting signal,
    // And to listen for click on direction selectors
    float cx = rc.CmToPixel((float)anchor.x+(float)bluePrint.Width/2.0f);
    float cy = rc.CmToPixel((float)anchor.y + (float)bluePrint.Height/2.0f);
    
    float t = Wrap(GetTime(),0.0f, 4.0f) * 90;
    // rotate every 4 second;

    DrawCircleSelectorLines({cx,cy}, rc.pixPr_cm*0.75f, 90+t, 270+t, 1, WHITE );
}


