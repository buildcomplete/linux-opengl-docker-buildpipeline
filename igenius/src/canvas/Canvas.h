#pragma once
#include <vector>
#include <queue>
#include "canvas/components/CanvasComponents.h"
#include "canvas/ComponentFactory.h"
#include "CoordinateHelper.h"
#include <cstdint>
#include <memory>
#include "Engine_StateManager.h"
#include "context/NavigationContext.h"
#include "context/RenderContext.h"
#include "context/StateContext.h"

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
// This we need 255  of
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
    bool AddComponent(const ComponentBluePrint &bluePrint, unsigned char cellAnchorX, unsigned char cellAnchorY);
    bool MoveComponentTo(std::uint8_t componentId, CellPosition newPosition);
    bool ReleaseComponent(std::uint8_t id);
    void Draw(const RenderContext &rc, const NavigationContext &navC, const StateContext &sc) const;

    void DrawOverlay(const RenderContext &rc, const NavigationContext &navC) const;

    bool IsGridFree(const ComponentBluePrint &blueprint, int cellX, int cellY) const;
    bool IsGridFree(const ComponentBluePrint &blueprint, int cellX, int cellY, std::uint8_t componentIdToIgnore) const;
    std::unique_ptr<CanvasComponentBase> components[256];
    GridContentInfo GetCellInfo(CellPosition cp) const;
    GridContentInfo GetCellInfo(int cellX, int cellY) const;
    bool HaveComponent(std::uint8_t id) const;
    CanvasComponentBase *GetComponent(std::uint8_t id) const;
    void SetGridCellValues(const ComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, std::uint8_t id);
    NetworkConstraintFlags GetNetworkConstraints(const CellPosition &anchor) const;
    void SetConstraints(const ComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, NetworkConstraintFlags constraintFlags);
    NetworkCheckState CheckNetwork(std::vector<CellPosition> &anchorPoints, std::uint8_t id) const;
    
    // Add a network, if id is zero, return 0 if failed adding, otherwise segmentIdentifier (not network id)
    std::uint32_t AddNetworkSegment(std::vector<CellPosition> &anchorPoints, std::uint8_t id); 
    bool RemoveNetworkSegment(std::uint32_t segmentIdentifier); 
    static void DrawNetworkSegment(const std::vector<CellPosition> &network, bool drawNodes, float pixPr_cm, Color c);

    ~Canvas();

private:
    std::vector<CellPosition> GetNetworkSamplePositions(std::vector<CellPosition> &anchorPoints) const;

    void DrawBounds(const NavigationContext &navC, float limRCM, float limBCM) const;
    static const std::uint8_t GridWidth = 255;
    static const std::uint8_t GridHeight = 255;
    void SetGridCellContentInfo(const ComponentBluePrint &blueprint, std::uint8_t cellX, std::uint8_t cellY, GridContentInfo info);
    int GetGridIdxAtCell(int cellX, int cellY) const;

    // GridRegisterInfo gridComponentRegister[GridWidth * GridHeight] = {}; // Register of item types in the grid and reference to the contentInfo, zero initialized
    GridContentInfo gridContentInfo[GridWidth * GridHeight] = {0}; // Register of ids placed in the grid
    NetworkConstraintFlags networkConstraints[GridWidth * GridHeight] = {FLAG_NCONSTRAINT_NO_CONSTRAINTS}; // Register of network constraints
    std::set<std::uint8_t> avaliableComponentKeys;
    std::set<std::uint8_t> avaliableNetworkKeys;
    std::set<std::uint8_t> inUseComponentKeys;
    std::set<std::uint8_t> inUseNetworkKeys;

   
    // Realized networks
    std::vector<std::vector<CellPosition>> networks = std::vector<std::vector<CellPosition>>(); 
    std::uint32_t networkSegmentId=0;
    std::vector<std::uint32_t> networkSegmentIds = std::vector<std::uint32_t>(); 

};
