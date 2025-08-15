#ifndef IG_CANVAS_H
#define IG_CANVAS_H
#include <vector>
#include <queue>
#include "Component.h"
#include "CoordinateHelper.h"

typedef enum
{
    // read from a folder, but inputs defined from code, not ui, behaves like a simple camera (start / stop)
    UNDEFINED = 0,
    VIRTUAL_CAMERA = 1,
    SP_CONVOLUTION = 2,
    IMG_IMG_IMG_OPERATION = 3,
} CMPNAMES;

struct UIComponentBluePrint
{
    CMPNAMES Name;
    unsigned char Width;
    unsigned char Heigh;
};

/**
 * Represents the drawing area for components and manages a grid structure
 * to track item types in each cell for rendering and interaction.
 */
class Canvas
{
public:
    Canvas();
    UIComponentBluePrint GetBluePrint(CMPNAMES name);
    bool AddComponent(CMPNAMES name, unsigned char cellAnchorX, unsigned char cellAnchorY);
    void Update();
    void Draw(CoordinateHelper &);
    bool IsGridFree(const UIComponentBluePrint &blueprint, int cellX, int cellY);
    std::vector<UI_Component> components;
    int GetGridIdAtCell(int cellX, int cellY);
    unsigned char GetGridValueAtCell(int cellX, int cellY);
    static const unsigned char GridWidth = 255;
    static const unsigned char GridHeight = 255;

private:
    void SetGridCellValues(const UIComponentBluePrint &blueprint, unsigned char cellX, unsigned char cellY, unsigned char id);

    unsigned char gridContentRegister[GridWidth * GridHeight] = {0};
    std::queue<unsigned char> avaliableKeys;
    std::vector<UI_Component> uiComponents;
    std::vector<UIComponentBluePrint> uiComponentBluePrints = {
        {VIRTUAL_CAMERA, 1, 1},
        {SP_CONVOLUTION, 2, 1},
        {IMG_IMG_IMG_OPERATION, 1, 1},
    };
};

struct CellPosition
{
    int x;
    int y;
};


#endif