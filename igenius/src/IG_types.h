#pragma once
#include <cstdint>
#include <iostream>
#include <functional>

#define OLIVE_GREEN_DARK CLITERAL(Color){17, 23, 00, 255} //  Background
#define OLIVE_GREEN CLITERAL(Color){27, 33, 00, 255}      //  Background
#define NETGREEN CLITERAL(Color){65, 228, 22, 255}        // Network
#define ELECTRIC_BLUE CLITERAL(Color){94, 255, 254, 255}  // Network

typedef enum : std::uint32_t
{
    DT_NONE = 0,
    DT_SCALAR = 1 << 0,
    DT_IMAGE = 1 << 1,
    DT_BINARY_IMAGE = 1 << 2,
    DT_RAY = 1 << 3,
    DT_LINESEGMENT = 1 << 4,
    DT_CIRCLES = 1 << 5,
    DT_VERTICES = 1 << 6,
    DT_IMAGESET = 1 << 7,
    DT_PROFILEWITHORIGIN = 1 << 8,
    DT_IS_OPTIONAL = 1 << 30
} IGDataTypes;

typedef enum : std::uint32_t
{
    IG_INPUT_ZERO = 0,
    IG_INPUT_TRY_COMMAND = 1 << 0,        // 1 - Try to execute command from mouse cursor depending on state...
    IG_INPUT_SCREEN_DRAGGING = 1 << 1, // 2

    // Sticky cursor modes (mutually exclusive within this group)
    IG_INPUT_MODE_SELECTING = 1 << 2,       // 4 - selection/interaction, this can open context menu / switch component ui settings etc.
    IG_INPUT_MODE_DRAW_NETWORK = 1 << 3,    // 8 -  Draw network connections
    IG_INPUT_MODE_PLACE_COMPONENT = 1 << 4, // 16 - Place new component, when entering state, start component selection, probably starting to open the radial menu for components, alernative it should be a selection panel to the side while component can be selected, not clear yet
    IG_INPUT_MODE_MOVE_COMPONENT = 1 << 5,  // 32 - Move existing component, when a component is picked, set the pick id and moved, meaning we need a state that we are 'executing' or 'moving'
    IG_INPUT_MODE_DELETE = 1 << 6,         // 64 - Delete component/network, on click removes a component / network

    IG_INPUT_SELECT_MODE = 1 << 30 // Bit to start selecting mode mouse
    // UI state flags
    // UI_RADIAL_MENU_OPEN = 1 << 12,      // 4096 - Radial menu is visible
    // UI_COMPONENT_PALETTE_OPEN = 1 << 13 // 8192 - Component palette is visible

} INPUT_STATE_FLAGS;

struct CellPosition
{
    int x;
    int y;

    bool operator==(const CellPosition &rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const CellPosition &rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }

    CellPosition operator+(const CellPosition &rhs) const
    {
        return {x + rhs.x, y + rhs.y};
    }

    CellPosition operator-(const CellPosition &rhs) const
    {
        return {x - rhs.x, y - rhs.y};
    }
};

// Network connection constraints
typedef enum
{
    FLAG_NCONSTRAINT_NO_CONSTRAINTS = 0, // (0000 0000)
    FLAG_NCONSTRAINT_N = 1 << 0,         // 1 (0000 0001)
    FLAG_NCONSTRAINT_NE = 1 << 1,        // 2 (0000 0010)
    FLAG_NCONSTRAINT_E = 1 << 2,         // 4 (0000 0100)
    FLAG_NCONSTRAINT_SE = 1 << 3,        // 8 (0000 1000)
    FLAG_NCONSTRAINT_S = 1 << 4,         // 16 (0001 0000)
    FLAG_NCONSTRAINT_SW = 1 << 5,        // 32 (0010 0000)
    FLAG_NCONSTRAINT_W = 1 << 6,         // 64 (0100 0000)
    FLAG_NCONSTRAINT_NW = 1 << 7,        // 128 (1000 0000)
    FLAG_NCONSTRAINT_ALLDIR = 0xFF       // 0xFF (1111 1111)
} NetworkConstraintFlags;
using ConstraintFunction = std::function<NetworkConstraintFlags(const CellPosition &)>;

std::ostream &operator<<(std::ostream &os, const CellPosition &pos);
