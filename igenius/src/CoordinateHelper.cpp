#include "CoordinateHelper.h"
#include <iostream>

CoordinateHelper::CoordinateHelper()
{
    // Grid Resolution is set to one cm
	int cmon = GetCurrentMonitor();
    std::cout << "Current monitor=" << cmon << ". W x H = " << GetMonitorWidth(cmon) <<  " x " << GetMonitorHeight(cmon) << std::endl;
	pixPr_cm = 10.0f * (float)GetMonitorWidth(cmon) / (float)GetMonitorPhysicalWidth(cmon) ; // pix/cm
}

float CoordinateHelper::CmToPixel(float cm)
{
    return pixPr_cm * cm;
}

