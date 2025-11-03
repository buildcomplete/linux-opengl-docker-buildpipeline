#include "menus/UI_RadialSP_ConvolutionOperationMenu.h"
#include <iostream>
#include <cmath>

const Color hoverGreen = {NETGREEN.r, NETGREEN.g, NETGREEN.b, 100};
const Color bgGrey = {ELECTRIC_BLUE.r, ELECTRIC_BLUE.g, ELECTRIC_BLUE.b, 100};
UI_RadialSPConvolutionperationMenu::UI_RadialSPConvolutionperationMenu(ClickHandlerRadialMenu clickHandler)
    : UI_RadialMenuBase(
          clickHandler,
          {
              {ELECTRIC_BLUE, bgGrey, hoverGreen, 1}, // '"SELECT"' // Defined in SegmentOperations
              {ELECTRIC_BLUE, bgGrey, hoverGreen, 2}, // '"Box 3"'
              {ELECTRIC_BLUE, bgGrey, hoverGreen, 3}, // '"Box 5"'
              {ELECTRIC_BLUE, bgGrey, hoverGreen, 4}, // '"Gauss 3",'
              {ELECTRIC_BLUE, bgGrey, hoverGreen, 5}, // '"Gauss 5"'
              {ELECTRIC_BLUE, bgGrey, hoverGreen, 6}  // '"Laplacian 1D"'
          },
          7,
          3)
{
}

void UI_RadialSPConvolutionperationMenu::DrawSegmentIcon(const RenderContext &rc, const NavigationContext &navCtx, int segmentIndex, float startAngle, float endAngle, float rInPx, float rOutPx) const
{
    DrawTextAtSegmentCenter(
        rc, navCtx, SegmentOperations[segmentIndex].c_str(), NETGREEN, 40, startAngle, endAngle, rInPx, rOutPx);
}