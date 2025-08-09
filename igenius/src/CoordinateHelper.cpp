#include "CoordinateHelper.h"

CoordinateHelper::CoordinateHelper()
{
    // Grid Resolution is set to one cm
	int cmon = GetCurrentMonitor();
	res = 10.0f * (float)GetMonitorWidth(cmon) / (float)GetMonitorPhysicalWidth(cmon) ; // pix/cm

}

void CoordinateHelper::Draw(Vector2 gameMousePos, Camera2D camera)
{
    		// draw circle close to where mouse is
		Vector2 worldPos = GetScreenToWorld2D(gameMousePos, camera);

    Vector2 quantifiedCenter =  { 
        (float)round(worldPos.x / res) * res,
        (float)round(worldPos.y / res) * res};

    for (int r = -2;r<3;++r) {
        for (int c = -2;c<3;++c) {
            Vector2 indicatorPos = {
                quantifiedCenter.x + c*res,
                quantifiedCenter.y + r*res };

            Color circleColor = WHITE;
            float alpha = Clamp(
                1.0f-Vector2Distance(worldPos, indicatorPos) / (2.5f * res), 
                0.0f, 
                1.0f);
            
            circleColor.a = (int)floor(255.0f * alpha);

            DrawCircleLines( indicatorPos.x, indicatorPos.y, res/6.0f, circleColor );
        }
    }
}