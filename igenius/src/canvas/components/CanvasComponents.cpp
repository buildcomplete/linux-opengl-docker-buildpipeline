#include "canvas/components/CanvasComponents.h"
#include <iostream>
#include <algorithm>
#include "rshapes.c"

// Small copy paste from raylib to support drawing line segments in cm space with offset
// Draw lines sequence (using gl lines)
static void DrawLineStripCMPro(const Vector2 *points, int pointCount, Color color, float pixPr_cm, const Vector2 offsetCm)
{
    if (pointCount < 2)
        return; // Security check

    rlBegin(RL_LINES);
    rlColor4ub(color.r, color.g, color.b, color.a);

    for (int i = 0; i < pointCount - 1; i++)
    {
        rlVertex2f((offsetCm.x + points[i].x) * pixPr_cm, (offsetCm.y + points[i].y) * pixPr_cm);
        rlVertex2f((offsetCm.x + points[i + 1].x) * pixPr_cm, (offsetCm.y + points[i + 1].y) * pixPr_cm);
    }
    rlEnd();
}

// Mod of DrawCircleSectorLines as I dont wont to draw a kind of thumb button
// Draw a piece of a circle outlines
void DrawCircleSelectorLines(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color)
{
    if (startAngle == endAngle)
        return;
    if (radius <= 0.0f)
        radius = 0.1f; // Avoid div by zero issue

    // Function expects (endAngle > startAngle)
    if (endAngle < startAngle)
    {
        // Swap values
        float tmp = startAngle;
        startAngle = endAngle;
        endAngle = tmp;
    }

    int minSegments = (int)ceilf((endAngle - startAngle) / 90);

    if (segments < minSegments)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        float th = acosf(2 * powf(1 - SMOOTH_CIRCLE_ERROR_RATE / radius, 2) - 1);
        segments = (int)((endAngle - startAngle) * ceilf(2 * PI / th) / 360);

        if (segments <= 0)
            segments = minSegments;
    }

    float stepLength = (endAngle - startAngle) / (float)segments;
    float angle = startAngle;
    bool showCapLines = true;
    float opositeAngle = endAngle + 90;
    rlBegin(RL_LINES);
    if (showCapLines)
    {
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex2f(center.x + cosf(DEG2RAD * opositeAngle) * radius, center.y + sinf(DEG2RAD * opositeAngle) * radius);
        rlVertex2f(center.x + cosf(DEG2RAD * angle) * radius, center.y + sinf(DEG2RAD * angle) * radius);
    }

    for (int i = 0; i < segments; i++)
    {
        rlColor4ub(color.r, color.g, color.b, color.a);

        rlVertex2f(center.x + cosf(DEG2RAD * angle) * radius, center.y + sinf(DEG2RAD * angle) * radius);
        rlVertex2f(center.x + cosf(DEG2RAD * (angle + stepLength)) * radius, center.y + sinf(DEG2RAD * (angle + stepLength)) * radius);

        angle += stepLength;
    }

    if (showCapLines)
    {
        rlColor4ub(color.r, color.g, color.b, color.a);
        rlVertex2f(center.x + cosf(DEG2RAD * opositeAngle) * radius, center.y + sinf(DEG2RAD * opositeAngle) * radius);
        rlVertex2f(center.x + cosf(DEG2RAD * angle) * radius, center.y + sinf(DEG2RAD * angle) * radius);
    }
    rlEnd();
}

CanvasComponentBase::CanvasComponentBase(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
    : id(id_), anchor(anchor_), bluePrint(bluePrint_)
{
}

void CanvasComponentBase::Draw(const RenderContext &rc, const NavigationContext &navCtx) const
{
    const float padding_cm = 0.1f;
    DrawStandardComponentFrame(rc, navCtx, padding_cm);

    DrawStandardSockets(rc, navCtx, padding_cm);
}

void CanvasComponentBase::FocusedDraw(const RenderContext &rc, const NavigationContext &navCtx ) const { }

Vector2 CanvasComponentBase::GetCursorOffset(const NavigationContext &navCtx)
{
    std::cout 
        << "CanvasComponentBase::GetCursorOffset " 
        << navCtx.mousePosWorldCm.x << "; "
        << navCtx.mousePosWorldCm.y << "; "
        << anchor.x << "; "
        << anchor.y << "; "
        <<  std::endl;
    return Vector2Subtract(navCtx.mousePosWorldCm, {(float)anchor.x, (float)anchor.y});
}

bool CanvasComponentBase::TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx)
{
    Vector2 offset = GetCursorOffset(navCtx);
    std::cout << "Clicked component: " << id << " mousePosWorldCm: " << navCtx.mousePosWorldCm.x << ", " << navCtx.mousePosWorldCm.y << std::endl;
    std::cout << "Clicked component: " << id << " anchor: " << anchor.x << ", " << anchor.y << std::endl;
    std::cout << "Clicked component: " << id << " cmp offset: " << offset.x << ", " << offset.y << std::endl;
    return false;
}

bool CanvasComponentBase::HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx)
{
    // Put code in these overrrides to to hanle events, including keypresses etc.
    // it would be nice to abstract away keypresses into named actions...
    // Those named actions are actually the stateContext, flipped
    // return true to keep focus for events, return false to release focus,
    // this method will not be called again before the same component returns true after TryStartCommand
    return false;
}

void CanvasComponentBase::DrawStandardComponentFrame(const RenderContext &rc, const NavigationContext &navCtx, const float padding_cm) const
{
    int xp = navCtx.CmToPixel(anchor.x + padding_cm);
    int yp = navCtx.CmToPixel(anchor.y + padding_cm);
    int wp = navCtx.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    int hp = navCtx.CmToPixel(bluePrint.Height - 2.0f * padding_cm);
    DrawRectangle(xp + 1, yp + 1, wp - 2, hp - 2, PINK);
}

// const std::vector<IOPin>& UI_Component::GetInputPins() const
// {
//     return inputs;
// }

void CanvasComponentBase::DrawStandardSockets(const RenderContext &rc, const NavigationContext &navCtx, float padding_cm) const
{
    int xp = navCtx.CmToPixel(anchor.x + padding_cm);
    int wp = navCtx.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    // Draw sockets on the edge as circles
    // In theory the sockets could be on any locations, but for simplicity we add them here
    // The socket shape depends on the type
    // The socket color changes with socket number

    // Input sockets on the left,
    if (bluePrint.inputDataTypes.size() != 0)
    {
        int ys0 = navCtx.CmToPixel(anchor.y);
        float inPosX = xp;
        for (int i = 0; i < bluePrint.inputDataTypes.size(); ++i)
        {

            float inPosY = ys0 + navCtx.pixPr_cm / 2.0f + ((float)i * 2) * navCtx.pixPr_cm;
            DrawCircleLinesV({inPosX, inPosY}, navCtx.pixPr_cm / 8.0f, WHITE);
        }
    }

    // output on the right
    if (bluePrint.outputDataType.type != DT_NONE)
    {
        float y_out = navCtx.CmToPixel((float)anchor.y + (float)bluePrint.Height / 2.0f);
        float outPosX = xp + wp;
        DrawCircleLinesV({outPosX, y_out}, navCtx.pixPr_cm / 8.0f, WHITE);
    }
}

CanvasComponents::VirtualCameraUI::VirtualCameraUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
    : CanvasComponentBase(id_, anchor_, bluePrint_) {}

void CanvasComponents::VirtualCameraUI::Draw(const RenderContext &rc, const NavigationContext &navCtx) const
{
    // Depending in state, we should draw either a start or stop button
    float x0 = navCtx.CmToPixel(anchor.x);
    float y0 = navCtx.CmToPixel(anchor.y);
    float wp = navCtx.CmToPixel(bluePrint.Width);
    float hp = navCtx.CmToPixel(bluePrint.Height);

    if (isCapturing)
    {
        float pad = navCtx.CmToPixel(0.3);
        float padX = navCtx.CmToPixel(0.1);
        const Color electricBlueTransparant = {ELECTRIC_BLUE.r, ELECTRIC_BLUE.g, ELECTRIC_BLUE.b, 50};
        DrawRectangle(x0 + pad + padX, y0 + pad, wp - 2 * pad, hp - 2 * pad, electricBlueTransparant);
    }

    DrawTexturePro(rc.cameraTexture,
                   {(float)0, (float)0, (float)rc.cameraTexture.width, (float)rc.cameraTexture.height},
                   {x0, y0, wp, hp},
                   {0, 0},
                   0,
                   WHITE);

    DrawStandardSockets(rc, navCtx, 0);
}

bool CanvasComponents::VirtualCameraUI::TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx)
{
    Vector2 offset = Vector2Subtract(navCtx.mousePosWorldCm, {(float)anchor.x, (float)anchor.y});
    std::cout << "Clicked camera: " << id << " cmp offset: " << offset.x << ", " << offset.y << std::endl;
    isCapturing = !isCapturing;
    return false;
}


CanvasComponents::IMG_IMG_IMG_OPERATIONUI::IMG_IMG_IMG_OPERATIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
    : CanvasComponentBase(id_, anchor_, bluePrint_) {}


void CanvasComponents::IMG_IMG_IMG_OPERATIONUI::Draw(const RenderContext &rc, const NavigationContext &navCtx) const
{
    const float padding_cm = 0.1f;
    DrawStandardComponentFrame(rc, navCtx, padding_cm);

    // Draw sockets on the edge as circles
    // In theory the sockets could be on any locations, but for simplicity we add them here
    // The socket shape depends on the type
    // The socket color changes with socket number

    DrawStandardSockets(rc, navCtx, padding_cm);
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
    float cx = navCtx.CmToPixel((float)anchor.x + (float)bluePrint.Width / 2.0f);
    float cy = navCtx.CmToPixel((float)anchor.y + (float)bluePrint.Height / 2.0f);

    float t = Wrap(GetTime(), 0.0f, 4.0f) * 90;
    // rotate every 4 second;

    DrawCircleSelectorLines({cx, cy}, navCtx.pixPr_cm * 0.75f, 90 + t, 270 + t, 1, WHITE);
}

CanvasComponents::DrawingSurfaceComponent::DrawingSurfaceComponent(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
    : CanvasComponentBase(id_, anchor_, bluePrint_) {}

void CanvasComponents::DrawingSurfaceComponent::Draw(const RenderContext &rc, const NavigationContext &navCtx) const
{
    const float padding_cm = 0.0f;
    int xp = navCtx.CmToPixel(anchor.x + padding_cm);
    int yp = navCtx.CmToPixel(anchor.y);
    int wp = navCtx.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    int hp = navCtx.CmToPixel(bluePrint.Height);
    DrawRectangleLines(xp + 1, yp + 1, wp - 2, hp - 2, ELECTRIC_BLUE);

    if (strokes.size() > 1)
    {
        DrawLineStripCMPro(&strokes[0], strokes.size(), WHITE, navCtx.pixPr_cm, {(float)anchor.x, (float)anchor.y});
    }

    if (strokes.size() > 0 && isDrawing)
    {
        DrawLineStripCMPro(&currentMouseMove[0], 2, YELLOW, navCtx.pixPr_cm, {(float)anchor.x, (float)anchor.y});
    }

    for (auto p : strokes)
    {
        DrawCircleLines(navCtx.CmToPixel(anchor.x + p.x), navCtx.CmToPixel(anchor.y + p.y), navCtx.pixPr_cm / 8.0f, YELLOW);
    }
}

bool CanvasComponents::DrawingSurfaceComponent::TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx)
{
    currentMouseMove[0] = GetCursorOffset(navCtx);
    currentMouseMove[1] = currentMouseMove[0];
    strokes.push_back(currentMouseMove[0]);
    isDrawing = true;
    return true;
}

bool CanvasComponents::DrawingSurfaceComponent::HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx)
{
    if (IsKeyPressed(KEY_C))
    {
        strokes.clear();
        isDrawing = false;
        return false;
    }

    if (IsKeyPressed(KEY_S))
    {
        // Save strokes, exit dedicated controll
        isDrawing = false;
        return false;
    }

    currentMouseMove[1] = Vector2Min({(float)bluePrint.Width, (float)bluePrint.Width}, GetCursorOffset(navCtx));

    return true;
}

