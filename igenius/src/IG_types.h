#ifndef IG_TYPES_H
#define IG_TYPES_H
#include <cstdint>

#define OLIVE_GREEN   	CLITERAL(Color){ 27, 33, 00, 255 }      //  Background
#define NETGREEN      	CLITERAL(Color){ 65, 228, 22, 255 }      // Network
#define ELECTRIC_BLUE   CLITERAL(Color){ 94, 255, 254, 255 }      // Network

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

struct CellPosition
{
    int x;
    int y;
};

struct IG_MouseCoordinates
{
    Vector2 mousePosScreenPixel;
    Vector2 mousePosWorldCm;
    CellPosition mousePosWorldGrid;
};


#endif