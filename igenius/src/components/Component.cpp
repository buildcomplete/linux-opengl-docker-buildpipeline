#include "Component.h"
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
        rlVertex2f((offsetCm.x + points[i].x) * pixPr_cm, (offsetCm.y + points[i].y) * pixPr_cm );
        rlVertex2f((offsetCm.x+points[i + 1].x) * pixPr_cm, (offsetCm.y+points[i + 1].y) * pixPr_cm);
    }
    rlEnd();
}

// Mod of DrawCircleSectorLines as I dont wont to draw a kind of thumb button
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

Vector2 UI_Component::GetCursorOffset( const NavigationContext& navCtx) 
{
    return  Vector2Subtract( navCtx.mousePosWorldCm, {(float)anchor.x, (float)anchor.y});
}

bool UI_Component::TryStartCommand(const StateContext &stCtx, const NavigationContext& navCtx)
{
    Vector2 offset =GetCursorOffset(navCtx);
    std::cout << "Clicked component: " << id << " mousePosWorldCm: " << navCtx.mousePosWorldCm.x << ", " << navCtx.mousePosWorldCm.y << std::endl;
    std::cout << "Clicked component: " << id << " anchor: " << anchor.x << ", " << anchor.y << std::endl;
    std::cout << "Clicked component: " << id << " cmp offset: " << offset.x << ", " << offset.y << std::endl;
    return false;
}

bool UI_Component::HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx)
{
   // Put code in these overrrides to to hanle events, including keypresses etc.
   // it would be nice to abstract away keypresses into named actions...
   // Those named actions are actually the stateContext, flipped
   // return true to keep focus for events, return false to release focus, 
   // this method will not be called again before the same component returns true after TryStartCommand
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
        float pad = rc.CmToPixel(0.3);
        float padX = rc.CmToPixel(0.1);
        const Color electricBlueTransparant = {ELECTRIC_BLUE.r, ELECTRIC_BLUE.g, ELECTRIC_BLUE.b, 50};
        DrawRectangle(x0+pad+padX, y0+pad, wp-2*pad,hp-2*pad, electricBlueTransparant );
    }

    DrawTexturePro(rc.cameraTexture, 
        {(float)0,(float)0,(float)rc.cameraTexture.width, (float)rc.cameraTexture.height},  
        {x0, y0, wp, hp},
        {0, 0},
        0,
        WHITE);

    
    DrawStandardSockets(rc, 0);
}

bool UI_Components::VirtualCameraUI::TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx)
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

UI_Components::DrawingSurfaceComponent::DrawingSurfaceComponent(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
     : UI_Component(id_, anchor_, bluePrint_) {}

void UI_Components::DrawingSurfaceComponent::Draw(const RenderContext &rc) const
{
    const float padding_cm = 0.0f;
    int xp = rc.CmToPixel(anchor.x + padding_cm);
    int yp = rc.CmToPixel(anchor.y);
    int wp = rc.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    int hp = rc.CmToPixel(bluePrint.Height);
    DrawRectangleLines(xp + 1, yp + 1, wp - 2, hp - 2, ELECTRIC_BLUE);

    if (strokes.size() > 1)
    {
        DrawLineStripCMPro(&strokes[0], strokes.size(), WHITE, rc.pixPr_cm, {(float)anchor.x, (float)anchor.y});
    }

    if (strokes.size() > 0 && isDrawing)
    {
        DrawLineStripCMPro(&currentMouseMove[0], 2, YELLOW, rc.pixPr_cm, {(float)anchor.x, (float)anchor.y});
    }

    for (auto p : strokes)
    {
        DrawCircleLines(rc.CmToPixel( anchor.x+ p.x), rc.CmToPixel( anchor.y+ p.y), rc.pixPr_cm/8.0f, YELLOW); 
    }
}

bool UI_Components::DrawingSurfaceComponent::TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx)
{
    currentMouseMove[0] = GetCursorOffset(navCtx);
    currentMouseMove[1] = currentMouseMove[0];
    strokes.push_back(currentMouseMove[0]);
    isDrawing=true;
    return true;
}

bool UI_Components::DrawingSurfaceComponent::HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx)
{
    if (IsKeyPressed(KEY_C))
    {
        strokes.clear();
        isDrawing=false;
        return false;
    }

    if (IsKeyPressed(KEY_S))
    {
        // Save strokes, exit dedicated controll
        isDrawing=false;
        return false;
    }
    
    currentMouseMove[1] = Vector2Min({(float)bluePrint.Width,(float)bluePrint.Width}, GetCursorOffset(navCtx));
    
    return true;
}

//UI_Selectors::DrawRadialMenu(const Vector2 center, float rStart, float rend, std::vector<std::fun


void UI_Selector::BeginExpand()
{
    startExpansionTime = GetTime();
    animState = UI_ANIM_STATE::UITEM_EXPANDING;
}

int UI_RadialMenuDrawingComponent::GetSelectedSegment(const Vector2 &origoCM, const NavigationContext &navC, float rInCm, float rOutCm, int segments)
{
    Vector2 origoToMouse = Vector2Subtract(origoCM, navC.mousePosWorldCm);
    float mouseAngle = -1.0f * RAD2DEG * Vector2LineAngle(origoCM, navC.mousePosWorldCm);
    float
        segmentDistSQMin = rInCm * rInCm,
        segmentDistSQMax = rOutCm * rOutCm,
        cursorOrigoeDistSQ = Vector2DistanceSqr({0, 0}, origoToMouse);

    if (segmentDistSQMin < cursorOrigoeDistSQ && cursorOrigoeDistSQ < segmentDistSQMax)
    {
        if (mouseAngle < 0)
            mouseAngle = 360 + mouseAngle;
        return (mouseAngle / 360.0) * segments;
    }
    return -1;
}

void UI_RadialMenuDrawingComponent::DrawRadialMenu(const NavigationContext &navC, const RenderContext &rc, const StateContext &sc, const Vector2& origoCM, int segments, double startTime, float scale, float rOutCm, float rInCm )
{
    float gapPx = rc.CmToPixel(0.2f * scale);
    // Calculate selected segment from mouse/cursor position
    int selectedSegment = UI_RadialMenuDrawingComponent::GetSelectedSegment(origoCM, navC, rInCm, rOutCm, segments);

    Vector2 origoPx = {rc.CmToPixel(origoCM.x), rc.CmToPixel(origoCM.y)};
    float r1Px = rc.CmToPixel(rOutCm);
    float r2Px = rc.CmToPixel(rInCm);
    
    float deltaV = 360.0f / (float)segments;

    std::string exampleSymbols = "+-/x|:";
    Color segmentColors[]{RED, ELECTRIC_BLUE, YELLOW, PURPLE, PINK};
    int nSegmentColors = 5;
    int subSegments = floor(18.0 / segments) + 1;

    float gapDegR1 = (gapPx / r1Px) * RAD2DEG; // Angular gap in degrees
    float gapDegR2 = (gapPx / r2Px) * RAD2DEG; // Angular gap in degrees

    // Calculate segment bounds, zero centered, so offset should be added when drawing.
    for (int i = 0; i < segments; ++i)
    {
        float startAngleReal = (float)i * deltaV;
        float endAngleReal = startAngleReal + deltaV;

        // Inset by half the gap on each side
        float startAngleR1 = startAngleReal + gapDegR1 * 0.5f;
        float endAngleR1 = endAngleReal - gapDegR1 * 0.5f;
        float subAngleR1 = (endAngleR1 - startAngleR1) / subSegments;

        float startAngleR2 = startAngleReal + gapDegR2 * 0.5f;
        float endAngleR2 = endAngleReal - gapDegR2 * 0.5f;
        float subAngleR2 = (endAngleR2 - startAngleR2) / subSegments;

        Color segmentColor = segmentColors[i % nSegmentColors];


            char buffer[150];
        sprintf(buffer, "Segments: %.1d DeltaVL %.1f selectedSegment: %d", segments, deltaV,  selectedSegment);
        DrawText(buffer, 20, 60, 20, WHITE);

        //if (startAngleReal < mouseAngle && mouseAngle < endAngleReal && segmentDistSQMin < cursorOrigoeDistSQ && cursorOrigoeDistSQ < segmentDistSQMax)
        if (selectedSegment == i)
        {
            segmentColor = NETGREEN;
        }

        float
            p1x = cosf(DEG2RAD * startAngleR1) * r1Px,
            p1y = sinf(DEG2RAD * startAngleR1) * r1Px,
            p2x = cosf(DEG2RAD * endAngleR1) * r1Px,
            p2y = sinf(DEG2RAD * endAngleR1) * r1Px,
            p3x = cosf(DEG2RAD * endAngleR2) * r2Px,
            p3y = sinf(DEG2RAD * endAngleR2) * r2Px,
            p4x = cosf(DEG2RAD * startAngleR2) * r2Px,
            p4y = sinf(DEG2RAD * startAngleR2) * r2Px;

        // Draw line between the inner and outer radius
        DrawLine(p1x + origoPx.x, p1y + origoPx.y, p4x + origoPx.x, p4y + origoPx.y, segmentColor);
        DrawLine(p2x + origoPx.x, p2y + origoPx.y, p3x + origoPx.x, p3y + origoPx.y, segmentColor);

        // Draw segments defining each outer radius
        for (int ii = 0; ii < subSegments; ++ii)
        {
            float startAngle2_R1 = startAngleR1 + subAngleR1 * (float)ii;
            float endAngle2_R1 = startAngleR1 + subAngleR1 * (float)(ii + 1);
            float startAngle2_R2 = startAngleR2 + subAngleR2 * (float)ii;
            float endAngle2_R2 = startAngleR2 + subAngleR2 * (float)(ii + 1);

            float
                pp1x = cosf(DEG2RAD * startAngle2_R1) * r1Px,
                pp1y = sinf(DEG2RAD * startAngle2_R1) * r1Px,
                pp2x = cosf(DEG2RAD * endAngle2_R1) * r1Px,
                pp2y = sinf(DEG2RAD * endAngle2_R1) * r1Px,
                pp3x = cosf(DEG2RAD * endAngle2_R2) * r2Px,
                pp3y = sinf(DEG2RAD * endAngle2_R2) * r2Px,
                pp4x = cosf(DEG2RAD * startAngle2_R2) * r2Px,
                pp4y = sinf(DEG2RAD * startAngle2_R2) * r2Px;

            Vector2 p1 = {pp1x + origoPx.x, pp1y + origoPx.y};
            Vector2 p2 = {pp2x + origoPx.x, pp2y + origoPx.y};
            Vector2 p3 = {pp3x + origoPx.x, pp3y + origoPx.y};
            Vector2 p4 = {pp4x + origoPx.x, pp4y + origoPx.y};

            // Fill quad slice between inner/outer angles
            DrawTriangle(p1, p3, p2, {segmentColor.r, segmentColor.g, segmentColor.b, 100});
            DrawTriangle(p1, p4, p3, {segmentColor.r, segmentColor.g, segmentColor.b, 100});

            DrawLine(pp1x + origoPx.x, pp1y + origoPx.y, pp2x + origoPx.x, pp2y + origoPx.y, segmentColor);
            DrawLine(pp3x + origoPx.x, pp3y + origoPx.y, pp4x + origoPx.x, pp4y + origoPx.y, segmentColor);
        }
        // sprintf(buffer, "startAngle %f Endangle: %f", startAngle, endAngle );
        // DrawText(buffer, -220,80+20*i,20,WHITE);

        // Text positions
        float
            tx = cosf(DEG2RAD * (startAngleR1 + deltaV / 2.0f)) * (r1Px + r2Px) / 2.0 + origoPx.x,
            ty = sinf(DEG2RAD * (startAngleR1 + deltaV / 2.0f)) * (r1Px + r2Px) / 2.0 + origoPx.y;
        sprintf(buffer, "%c", exampleSymbols[i % exampleSymbols.length()]);
        int fSize = (int)ceil(120 * scale);
        int tw = MeasureText(buffer, fSize);
        DrawText(buffer, tx - tw / 2, ty - fSize / 2, fSize, NETGREEN);
    }
}
