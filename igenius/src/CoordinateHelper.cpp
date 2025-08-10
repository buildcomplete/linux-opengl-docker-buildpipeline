#include "CoordinateHelper.h"

CoordinateHelper::CoordinateHelper()
{
    // Grid Resolution is set to one cm
	int cmon = GetCurrentMonitor();
	pixPr_cm = 10.0f * (float)GetMonitorWidth(cmon) / (float)GetMonitorPhysicalWidth(cmon) ; // pix/cm
}

float CoordinateHelper::CmToPixel(float cm)
{
    return pixPr_cm * cm;
}

