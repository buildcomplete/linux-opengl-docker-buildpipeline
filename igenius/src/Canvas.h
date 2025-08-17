#ifndef IG_CANVAS_H
#define IG_CANVAS_H
#include <vector>
#include <queue>
#include "Component.h"
#include "CoordinateHelper.h"
#include <cstdint>

typedef enum : std::int32_t
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
    std::uint8_t Width;
    std::uint8_t Heigh;
};

// Commenting out to decrease level of indirection,
// if we are going to have more than 3 types of object class we can place,
// we can reduce memory be introducing this level of additional indirection
// typedef enum : std::uint8_t
// {
//     GF_CLEAR        = 0,
//     GF_COMPONENT    = 1 << 0,
//     GF_NETWORK      = 1 << 1,
// } GridFlags;

// // Grid content info is used to help access to meta data related to what is in a grid area
// // To get the actual data, request the specific ids from grid register info.
// // These we will have 255*255 of
// struct GridRegisterInfo
// {
//     GridFlags flags;
//     std::uint16_t registerIdx;
// };

// Reference to actual object ids.
// This we need 255 * 2 of
struct GridContentInfo
{
    std::uint8_t componentId;
    std::uint8_t networkId;
};

struct NetworkCheckState
{
    bool IsValid; 
    std::uint8_t finalId; // input id if not assigned to any other networks
    GridContentInfo networkStartNodeInfo;
    GridContentInfo networkEndNodeInfo;
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
    bool AddComponent(CMPNAMES name, std::uint8_t cellAnchorX, std::uint8_t cellAnchorY);
    void Update();
    void Draw(CoordinateHelper &);
    bool IsGridFree(const UIComponentBluePrint &blueprint, int cellX, int cellY);
    std::vector<UI_Component> components;
    GridContentInfo GetComponentInfoFor(int cellX, int cellY);
    void SetGridCellValues(const UIComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, std::uint8_t id);
    std::vector<CellPosition> GetNetworkSamplePositions(std::vector<CellPosition> &anchorPoints);
    NetworkCheckState CheckNetwork(std::vector<CellPosition> &anchorPoints, std::uint8_t id);
    bool AddNetworkSegment(std::vector<CellPosition> &anchorPoints, std::uint8_t id); // Add a network, if id is zero, assigns new id

private:
    static const std::uint8_t GridWidth = 255;
    static const std::uint8_t GridHeight = 255;

    void SetGridCellContentInfo(const UIComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, GridContentInfo info);

    int GetGridIdxAtCell(int cellX, int cellY);

    // GridRegisterInfo gridComponentRegister[GridWidth * GridHeight] = {}; // Register of item types in the grid and reference to the contentInfo, zero initialized
    GridContentInfo gridContentInfo[GridWidth * GridHeight] = {0}; // Register of ids placed in the grid
    std::queue<std::uint8_t> avaliableComponentKeys;
    std::queue<std::uint8_t> avaliableNetworkKeys;
    std::vector<UI_Component> uiComponents;
    std::vector<UIComponentBluePrint> uiComponentBluePrints = {
        {VIRTUAL_CAMERA, 1, 1},
        {SP_CONVOLUTION, 2, 1},
        {IMG_IMG_IMG_OPERATION, 1, 1},
    };
};


#endif