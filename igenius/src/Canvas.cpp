#include "Canvas.h"

Canvas::Canvas()
{
    // Define all input frames
    char objectId = 0;
    components.push_back(UI_Component(++objectId, {1.0f,1.0f,1.0f,1.0f} ));     // Test Component
    components.push_back(UI_Component(++objectId, {1.0f,8.0f,1.0f,1.0f} ));     // UIBoxComponent(1,8,1,1, ELECTRIC_BLUE), // Camera is 1x1, starting at 1x10
    components.push_back(UI_Component(++objectId, {4.0f,7.0f,2.0f,1.0f} ));     // UIBoxComponent(4,7,2,1, ELECTRIC_BLUE), // ConvolveHorz is 2x1, starting at 4x7
    components.push_back(UI_Component(++objectId, {4.0f,9.0f,2.0f,1.0f} ));     // UIBoxComponent(4,9,2,1, ELECTRIC_BLUE), // ConvolveHorz is 2x1, starting at 4x9
    components.push_back(UI_Component(++objectId, {9.0f,5.0f,2.0f,3.0f} ));      // UIBoxComponent(9,5,2,3, ELECTRIC_BLUE), // SquareConvHorz is 2x3, starting at 9x7
    components.push_back(UI_Component(++objectId, {9.0f,9.0f,2.0f,3.0f} ));    // UIBoxComponent(9,9,2,3, ELECTRIC_BLUE) // SquareConvolveHorz is 2x3, starting at 9x9
}

void Canvas::Update()
{
    // Update location of mouse according to screen, gameworld in Component and network coordinates


    // Perform detection of what objects are selected
}


// Select / deselect component
// Multiple components can be selected
// We could, read the state here, and handle the logic
// Or we could manage that outside in the engine, and just inform when an event occured
// centralized or decentralized...
void ToggleComponent()
{

}

void Canvas::Draw(CoordinateHelper& coordinateHelper)
{
    for (int i=0;i<components.size();++i)
    {
        // This is only updated when changing monitor, so could be a fixed calculation 
	    const float padding_cm = 0.1f;
        int xp = coordinateHelper.CmToPixel(components[i].anchor.x - padding_cm);
		int yp = coordinateHelper.CmToPixel(components[i].anchor.y - padding_cm);
		int wp = coordinateHelper.CmToPixel(components[i].anchor.width + 2.0f * padding_cm);
		int hp = coordinateHelper.CmToPixel(components[i].anchor.height + 2.0f * padding_cm);
        DrawRectangleLines( xp, yp, wp, hp, ELECTRIC_BLUE);
    }
}