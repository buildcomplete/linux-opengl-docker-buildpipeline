#ifndef IG_TYPES_H
#define IG_TYPES_H

#define OLIVE_GREEN   	CLITERAL(Color){ 27, 33, 00, 255 }      //  Background
#define NETGREEN      	CLITERAL(Color){ 65, 228, 22, 255 }      // Network
#define ELECTRIC_BLUE   CLITERAL(Color){ 94, 255, 254, 255 }      // Network

typedef enum 
{
    SCALAR,
    IMAGE,
    BINARY_IMAGE,
    RAY,
    LINESEGMENT,
    CIRCLES,
    VERTICES,
    IMAGESET,
    PROFILEWITHORIGIN
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