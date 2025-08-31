#include "components/UI_RadialIMGIMGOperationMenu.h"
#include <iostream>

const Color hoverGreen = {NETGREEN.r, NETGREEN.g, NETGREEN.b, 100};
const Color bgGrey = {ELECTRIC_BLUE.r, ELECTRIC_BLUE.g, ELECTRIC_BLUE.b, 100};
UI_RadialIMGIMGOperationMenu::UI_RadialIMGIMGOperationMenu(ClickHandler clickHandler_)
    : UI_RadialMenuDrawingComponent(
          clickHandler_,
          {
              {ELECTRIC_BLUE, bgGrey, hoverGreen}, // '+' // Defined in SegmentOperations
              {ELECTRIC_BLUE, bgGrey, hoverGreen}, // '-'
              {ELECTRIC_BLUE, bgGrey, hoverGreen}, // '/'
              {ELECTRIC_BLUE, bgGrey, hoverGreen}  // 'X'
          },
          7,
          3)
{
}


void UI_RadialIMGIMGOperationMenu::DrawSegmentIcon(const RenderContext &rc, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const
{
    float deltaV = endAngle - startAngle;
    Vector2 origoPx = {rc.CmToPixel(origoCM.x), rc.CmToPixel(origoCM.y)};
    float
        tx = cosf(DEG2RAD * (startAngle + deltaV / 2.0f)) * (rInPx + rOutPx) / 2.0 + origoPx.x,
        ty = sinf(DEG2RAD * (startAngle + deltaV / 2.0f)) * (rInPx + rOutPx) / 2.0 + origoPx.y;

    char buffer[2];
    sprintf(buffer, "%c", SegmentOperations[segmentIndex % SegmentOperations.length()]);
    int fSize = (int)ceil(120 * expansionProgress);
    int tw = MeasureText(buffer, fSize);
    DrawText(buffer, tx - tw / 2, ty - fSize / 2, fSize, NETGREEN);
}