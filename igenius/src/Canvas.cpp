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
bool Canvas::AddComponent(CMPNAMES name, unsigned char cellAnchorX, unsigned char cellAnchorY)
{
    if (avaliableKeys.empty()) 
        return false;
    
    UIComponentBluePrint bluePrint = GetBluePrint(name);

    // Add check if position is valid
    if (!IsGridFree(bluePrint, cellAnchorX, cellAnchorY))
        return false;
        

    unsigned char insertId = avaliableKeys.front();
    
    // Update lookup table
    gridContentRegister[GetGridIdAtCell(cellAnchorX, cellAnchorY)] = insertId;
    
    std::cout << "Add: " << (int)insertId << " " << (float)cellAnchorX << ", " << (float)cellAnchorY << ", " << (float)bluePrint.Width << ", " << (float)bluePrint.Heigh << std::endl;
    components.push_back(UI_Component(insertId, { (float)cellAnchorX, (float)cellAnchorY, (float)bluePrint.Width, (float)bluePrint.Heigh} ));
    avaliableKeys.pop();
    return true;
    
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

bool Canvas::IsGridFree(const UIComponentBluePrint& blueprint, unsigned char cellX, unsigned char cellY)
{
    int x0 = cellX; // inclusive
    int y0 = cellY; // // inclusive
    int x1 = x0 + blueprint.Width; // Exclusive
    int y1 = y0 + blueprint.Heigh; // Exclusive
    if (x1 >= GridWidth || y1 >= GridHeight )
        return false;

    for ( ;x0<x1;++x0)
        for ( ;y0<y1;++y0)
            if (gridContentRegister[GetGridIdAtCell(x0,x1) != 0])
                return false;
    return true;
}

// Sets grid cell values, notice, this fellow do not perform boundaris checks and assumes the where checed elsewhere
void Canvas::SetGridCellValues(const UIComponentBluePrint& blueprint, unsigned char cellX, unsigned char cellY, unsigned char id)
{
    int x0 = cellX; // inclusive
    int y0 = cellY; // // inclusive
    int x1 = x0 + blueprint.Width; // Exclusive
    int y1 = y0 + blueprint.Heigh; // Exclusive
    
    for ( ;x0<x1;++x0)
        for ( ;y0<y1;++y0)
            gridContentRegister[GetGridIdAtCell(x0,x1)] = id; 
}

unsigned char Canvas::GetGridIdAtCell(unsigned char cellX, unsigned char cellY)
{
    return cellX + cellY * GridWidth;
}
