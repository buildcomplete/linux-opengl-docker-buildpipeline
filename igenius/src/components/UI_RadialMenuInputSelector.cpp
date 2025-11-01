#include "components/UI_RadialMenuInputSelector.h"
#include <iostream>

const Color hoverGreen = {NETGREEN.r, NETGREEN.g, NETGREEN.b, 100};
const Color bgGrey = {ELECTRIC_BLUE.r, ELECTRIC_BLUE.g, ELECTRIC_BLUE.b, 100};
UI_RadialMenuInputSelector::UI_RadialMenuInputSelector(ClickHandler clickHandler)
    : UI_RadialMenuBase(
          clickHandler,
          {
              {ELECTRIC_BLUE, bgGrey, hoverGreen}, // '"Edge"' // Defined in SegmentOperations
              {ELECTRIC_BLUE, bgGrey, hoverGreen}, // '"NET"'
              {ELECTRIC_BLUE, bgGrey, hoverGreen}, // '"PLACE"'
              {ELECTRIC_BLUE, bgGrey, hoverGreen}, // '"MOVE",'
              {ELECTRIC_BLUE, bgGrey, hoverGreen}, // '"DELETE"'
          },
          7,
          3)
{
}

void UI_RadialMenuInputSelector::DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const
{
    float deltaV = endAngle - startAngle;
    Vector2 origoPx = {navCtx.CmToPixel(origoCM.x), navCtx.CmToPixel(origoCM.y)};
    float
        tx = cosf(DEG2RAD * (startAngle + deltaV / 2.0f)) * (rInPx + rOutPx) / 2.0 + origoPx.x,
        ty = sinf(DEG2RAD * (startAngle + deltaV / 2.0f)) * (rInPx + rOutPx) / 2.0 + origoPx.y;

    int fSize = (int)ceil(30 * expansionProgress);
    int tw = MeasureText(SegmentOperations[segmentIndex].c_str(), fSize);
    DrawText(SegmentOperations[segmentIndex].c_str(), tx - tw / 2, ty - fSize / 2, fSize, NETGREEN);

}