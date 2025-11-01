#include "UI_RadialMenuBase.h"
#include <cmath>
#include "raymath.h"


UI_RadialMenuBase::UI_RadialMenuBase(ClickHandler clickHandler_, std::vector<RadialMenuSegmentColors> segments_, float rOutCm_, float rInCm_)
    : UI_MenuBase(clickHandler_), segments(segments_), rOutCm(rOutCm_), rInCm(rInCm_)
{
}

void UI_RadialMenuBase::HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx) 
{
    UI_MenuBase::HandleEventsAndTime( stCtx, navCtx);
    
    if (UI_ANIM_STATE::UITEM_OPEN == animState )
    {
        // update segment id
        hoveredSegment = UI_RadialMenuBase::GetHoverSegment(navCtx);

        // handle click
        if (stCtx.DidEnterState(INPUT_STATE_FLAGS::IG_INPUT_TRY_COMMAND) && hoveredSegment != -1)
        {
            selectedSegment = hoveredSegment;
            HandleClickInternal(selectedSegment);
        }
    }
};

int UI_RadialMenuBase::GetHoverSegment(const NavigationContext &navC) const 
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
        return (mouseAngle / 360.0) * segments.size();
    }
    return -1;
}

// Static main function, to be changed to get drawing functions instead of "int segments"
void UI_RadialMenuBase::DrawRadialMenu(const RenderContext &rc, const NavigationContext &navCtx, int nSegments) const 
{
    float gapPx = navCtx.CmToPixel(0.2f * expansionProgress);
    // Calculate selected segment from mouse/cursor position
   

    Vector2 origoPx = {navCtx.CmToPixel(origoCM.x), navCtx.CmToPixel(origoCM.y)};
    float r1Px = navCtx.CmToPixel(rOutCm) * expansionProgress;
    float r2Px = navCtx.CmToPixel(rInCm) * expansionProgress;

    float deltaV = 360.0f / (float)nSegments;

    
    int subSegments = floor(18.0 / nSegments) + 1;

    float gapDegR1 = (gapPx / r1Px) * RAD2DEG; // Angular gap in degrees
    float gapDegR2 = (gapPx / r2Px) * RAD2DEG; // Angular gap in degrees

    // Calculate segment bounds, zero centered, so offset should be added when drawing.
    for (int i = 0; i < nSegments; ++i)
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

        auto segmentColor = segments[i];
        Color bgColor = (i!=hoveredSegment) ? segmentColor.backgroundColor : segmentColor.hoverColor;

        char buffer[150];
        sprintf(buffer, "Segments: %.1d DeltaVL %.1f selectedSegment: %d", nSegments, deltaV, hoveredSegment);
        DrawText(buffer, 20, 60, 20, WHITE);


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
        DrawLine(p1x + origoPx.x, p1y + origoPx.y, p4x + origoPx.x, p4y + origoPx.y, segmentColor.edgeColor);
        DrawLine(p2x + origoPx.x, p2y + origoPx.y, p3x + origoPx.x, p3y + origoPx.y, segmentColor.edgeColor);

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
            DrawTriangle(p1, p3, p2, bgColor);
            DrawTriangle(p1, p4, p3, bgColor);

            DrawLine(pp1x + origoPx.x, pp1y + origoPx.y, pp2x + origoPx.x, pp2y + origoPx.y, segmentColor.edgeColor);
            DrawLine(pp3x + origoPx.x, pp3y + origoPx.y, pp4x + origoPx.x, pp4y + origoPx.y, segmentColor.edgeColor);
        }

        DrawSegmentIcon(rc, navCtx, i, startAngleReal, endAngleReal, r1Px, r2Px);
    }
}

void UI_RadialMenuBase::Draw(const RenderContext &rc, const NavigationContext &navCtx) const 
{
    if (UI_ANIM_STATE::UITEM_CLOSED == animState)
        return;

    DrawRadialMenu(rc, navCtx, segments.size());
}
