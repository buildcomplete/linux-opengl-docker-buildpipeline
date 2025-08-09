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


void CoordinateHelper::DrawHelpers(Vector2 gameMousePos, Camera2D camera)
{
    // Assuming drawing in camera mode.
    // draw circle close to where mouse is
    Vector2 worldPos = GetScreenToWorld2D(gameMousePos, camera);

    Vector2 quantifiedCenter =  { 
        (float)round(worldPos.x / pixPr_cm) * pixPr_cm,
        (float)round(worldPos.y / pixPr_cm) * pixPr_cm};

    for (int r = -2;r<3;++r) {
        for (int c = -2;c<3;++c) {
            Vector2 indicatorPos = {
                quantifiedCenter.x + c*pixPr_cm,
                quantifiedCenter.y + r*pixPr_cm };

            Color circleColor = WHITE;
            float alpha = Clamp(
                1.0f - Vector2Distance(worldPos, indicatorPos) / (2.5f * pixPr_cm), 
                0.0f, 
                1.0f);
            
            circleColor.a = (int)floor(255.0f * alpha);

            DrawCircleLines( indicatorPos.x, indicatorPos.y, pixPr_cm / 6.0f, circleColor );
        }
    }
}