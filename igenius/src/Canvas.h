#ifndef IG_CANVAS_H
#define IG_CANVAS_H
#include <vector>
#include <queue>
#include "Component.h"
#include "CoordinateHelper.h"
#include <cstdint>
#include "ComponentFactory.h"

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
    void Update();
    bool AddComponent(const ComponentBluePrint &bluePrint, unsigned char cellAnchorX, unsigned char cellAnchorY);
    void Draw(CoordinateHelper &coordinateHelper, bool drawAnchors);
    bool IsGridFree(const ComponentBluePrint &blueprint, int cellX, int cellY);
    UI_Component* components[256] = {nullptr}; 
    GridContentInfo GetCellInfo(int cellX, int cellY) const;
    void SetGridCellValues(const ComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, std::uint8_t id);
    std::vector<CellPosition> GetNetworkSamplePositions(std::vector<CellPosition> &anchorPoints);
    NetworkCheckState CheckNetwork(std::vector<CellPosition> &anchorPoints, std::uint8_t id);
    bool AddNetworkSegment(std::vector<CellPosition> &anchorPoints, std::uint8_t id); // Add a network, if id is zero, assigns new id
    static void DrawNetworkSegment(std::vector<CellPosition> &network, bool drawNodes, float pixPr_cm);

    ~Canvas();

private:
    static const std::uint8_t GridWidth = 255;
    static const std::uint8_t GridHeight = 255;

    void SetGridCellContentInfo(const ComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, GridContentInfo info);

    int GetGridIdxAtCell(int cellX, int cellY) const;

    // GridRegisterInfo gridComponentRegister[GridWidth * GridHeight] = {}; // Register of item types in the grid and reference to the contentInfo, zero initialized
    GridContentInfo gridContentInfo[GridWidth * GridHeight] = {0}; // Register of ids placed in the grid
    std::set<std::uint8_t> avaliableComponentKeys;
    std::set<std::uint8_t> avaliableNetworkKeys;
    std::set<std::uint8_t> inUseComponentKeys;
    std::set<std::uint8_t> inUseNetworkKeys;
   
    // Realized networks
    std::vector<std::vector<CellPosition>> networks = std::vector<std::vector<CellPosition>>(5); 

};


#endif