#include "menus/UI_RadialMenuIMGIMGOperation.h"
#include <iostream>
#include <cmath>

const Color hoverGreen = {NETGREEN.r, NETGREEN.g, NETGREEN.b, 100};
const Color bgGrey = {ELECTRIC_BLUE.r, ELECTRIC_BLUE.g, ELECTRIC_BLUE.b, 100};
UI_RadialMenuIMGIMGOperation::UI_RadialMenuIMGIMGOperation(ClickHandlerRadialMenu clickHandler_)
    : UI_RadialMenuBase(
          clickHandler_,
          {
              {ELECTRIC_BLUE, bgGrey, hoverGreen, '+'},
              {ELECTRIC_BLUE, bgGrey, hoverGreen, '-'},
              {ELECTRIC_BLUE, bgGrey, hoverGreen, '/'},
              {ELECTRIC_BLUE, bgGrey, hoverGreen, 'X'} 
          },
          7,
          3)
{
}

void UI_RadialMenuIMGIMGOperation::DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const
{
    char buffer[2];
    sprintf(buffer, "%c", (char)(segments[segmentIndex].yourId));
    DrawTextAtSegmentCenter(rc, navCtx, buffer, NETGREEN, 120, startAngle, endAngle, rInPx, rOutPx);
}