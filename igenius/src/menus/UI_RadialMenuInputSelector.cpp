#include "menus/UI_RadialMenuInputSelector.h"
#include <iostream>
#include <cmath>

const Color hoverGreen = {NETGREEN.r, NETGREEN.g, NETGREEN.b, 100};
const Color bgGrey = {ELECTRIC_BLUE.r, ELECTRIC_BLUE.g, ELECTRIC_BLUE.b, 100};
UI_RadialMenuInputSelector::UI_RadialMenuInputSelector(ClickHandlerRadialMenu clickHandler)
    : UI_RadialMenuBase(
          clickHandler,
          {
              {ELECTRIC_BLUE, bgGrey, hoverGreen, INPUT_STATE_FLAGS::IG_INPUT_MODE_SELECTING},
              {ELECTRIC_BLUE, bgGrey, hoverGreen, INPUT_STATE_FLAGS::IG_INPUT_MODE_DRAW_NETWORK},
              {ELECTRIC_BLUE, bgGrey, hoverGreen, INPUT_STATE_FLAGS::IG_INPUT_MODE_PLACE_COMPONENT},
              {ELECTRIC_BLUE, bgGrey, hoverGreen, INPUT_STATE_FLAGS::IG_INPUT_MODE_MOVE_COMPONENT},
              {ELECTRIC_BLUE, bgGrey, hoverGreen, INPUT_STATE_FLAGS::IG_INPUT_MODE_DELETE},
          },
          7,
          3)
{
}

void UI_RadialMenuInputSelector::DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const
{
    const char* text = "";
    switch ((INPUT_STATE_FLAGS)(segments[segmentIndex].yourId))
    {
    case INPUT_STATE_FLAGS::IG_INPUT_MODE_SELECTING:
        text = "SEL";
        break;
    case INPUT_STATE_FLAGS::IG_INPUT_MODE_DRAW_NETWORK:
        text = "NET";
        break;
    case INPUT_STATE_FLAGS::IG_INPUT_MODE_MOVE_COMPONENT:
        text = "MOV";
        break;
    case INPUT_STATE_FLAGS::IG_INPUT_MODE_PLACE_COMPONENT:
        text = "PUT";
        break;
    case INPUT_STATE_FLAGS::IG_INPUT_MODE_DELETE:
        text = "DEL";
        break;
    
    default:
        break;
    }

    float deltaV = endAngle - startAngle;
    Vector2 origoPx = {navCtx.CmToPixel(origoCM.x), navCtx.CmToPixel(origoCM.y)};
    float
        tx = cosf(DEG2RAD * (startAngle + deltaV / 2.0f)) * (rInPx + rOutPx) / 2.0 + origoPx.x,
        ty = sinf(DEG2RAD * (startAngle + deltaV / 2.0f)) * (rInPx + rOutPx) / 2.0 + origoPx.y;

    int fSize = (int)ceil(30 * expansionProgress);
    int tw = MeasureText(text, fSize);
    DrawText(text, tx - tw / 2, ty - fSize / 2, fSize, NETGREEN);

}