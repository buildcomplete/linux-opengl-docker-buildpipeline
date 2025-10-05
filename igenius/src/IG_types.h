#ifndef IG_TYPES_H
#define IG_TYPES_H
#include <cstdint>
#include <iostream>


#define OLIVE_GREEN_DARK  	CLITERAL(Color){ 17, 23, 00, 255 }      //  Background
#define OLIVE_GREEN   	    CLITERAL(Color){ 27, 33, 00, 255 }      //  Background
#define NETGREEN        	CLITERAL(Color){ 65, 228, 22, 255 }      // Network
#define ELECTRIC_BLUE       CLITERAL(Color){ 94, 255, 254, 255 }      // Network

typedef enum : std::uint32_t
{
    DT_NONE                = 0,
    DT_SCALAR              = 1 << 0,
    DT_IMAGE               = 1 << 1,
    DT_BINARY_IMAGE        = 1 << 2,
    DT_RAY                 = 1 << 3,
    DT_LINESEGMENT         = 1 << 4,
    DT_CIRCLES             = 1 << 5,
    DT_VERTICES            = 1 << 6,
    DT_IMAGESET            = 1 << 7,
    DT_PROFILEWITHORIGIN   = 1 << 8
} IGDataTypes;

typedef enum : std::uint32_t
{
    IG_MOUSE_ZERO                   = 0,
    IG_MOUSE_SELECTING 			    = 1 << 0, //1
    IG_MOUSE_SCREEN_DRAGGING 		= 1 << 1, // 2
    IG_MOUSE_MODE_NETWORK			= 1 << 2, // 4
    // Try to execute command from mouse cursor depending on state...
    IG_MOUSE_TRY_COMMAND			= 1 << 3, // 8
    IG_MOUSE_POSITION_COMPONENT 	= 1 << 4, // 16
    IG_MOUSE_DELETE                 = 1 << 5, // 32
    IG_MOUSE_APPLYING_COMMAND       = 1 << 6 // 64

} MOUSE_MODE_FLAGS;

struct CellPosition
{
    int x;
    int y;

    bool operator==(const CellPosition& rhs) const
    {
        return x==rhs.x&&y==rhs.y;
    }
    bool operator!=(const CellPosition& rhs) const
    {
        return x!=rhs.x||y!=rhs.y;
    }

    CellPosition operator+(const CellPosition& rhs) const
    {
        return {x+rhs.x, y+rhs.y};
    }
    
    CellPosition operator-(const CellPosition& rhs) const
    {
        return {x-rhs.x, y-rhs.y};
    }
};

std::ostream& operator<<(std::ostream& os, const CellPosition& pos);

#endif