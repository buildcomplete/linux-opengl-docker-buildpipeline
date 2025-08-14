#include "Canvas.h"
#include <iostream>

Canvas::Canvas()
{
     unsigned char i=0;
    do
    {
        i++;
        avaliableKeys.push(i);
        /* code */
    } while (i!=255);
    
    // Define all input frames
    AddComponent(CMPNAMES::UNDEFINED, 1,1);
    AddComponent(CMPNAMES::VIRTUAL_CAMERA, 1,8);
    AddComponent(CMPNAMES::SP_CONVOLUTION, 4,7);
    AddComponent(CMPNAMES::SP_CONVOLUTION, 4,9);
    AddComponent(CMPNAMES::IMG_IMG_IMG_OPERATION, 9,5);
    AddComponent(CMPNAMES::IMG_IMG_IMG_OPERATION, 9,9);
}

UIComponentBluePrint Canvas::GetBluePrint(CMPNAMES name)
{
    for (int i=0;i<uiComponentBluePrints.size(); ++i)
    {
        if (uiComponentBluePrints[i].Name == name)
            return uiComponentBluePrints[i];
    }
    return {UNDEFINED, 1, 1};
}
bool Canvas::AddComponent(CMPNAMES name, int cellAnchorX, int cellAnchorY)
{
    if (!avaliableKeys.empty()) 
    {
        char insertId = avaliableKeys.front();
        // Add check if position is valid
        UIComponentBluePrint bluePrint = GetBluePrint(name);
        std::cout << "Add: " << (int)insertId << " " << (float)cellAnchorX << ", " << (float)cellAnchorY << ", " << (float)bluePrint.Width << ", " << (float)bluePrint.Heigh << std::endl;
        components.push_back(UI_Component(insertId, { (float)cellAnchorX, (float)cellAnchorY, (float)bluePrint.Width, (float)bluePrint.Heigh} ));
        avaliableKeys.pop();
        return true;
    }
    return false;
    
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