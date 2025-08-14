#ifndef IG_ENGINE_H
#define IG_ENGINE_H

#include "Engine_StateManager.h"
#include "Navigator.h"
#include "CoordinateHelper.h"
#include "Canvas.h"
#include "Engine_NetworkDrawingManager.h"

class UIBoxComponent
{
public:	
	
	int x,y,w, h; // Grid aligned (cm) base coordinates
	Color color;
	UIBoxComponent(int _x, int _y, int _w, int _h, Color _color) 
		: x(_x), y(_y), w(_w), h(_h),color(_color)
	{
		
	}

	bool PointInside(Vector2 p)
	{
		return CheckCollisionPointRec(p, {(float)x,(float)y,(float)w,(float)h});
	}

	// Calculate how the Linesegment defined from a to b intersects the borders of the components 
	// of this box.
	// If there is intersection
	Vector2 Intersection(Vector2 a, Vector2 b)
	{
		
	}

	void Draw(CoordinateHelper& coordinateHelper)
	{
		// This is only updated when changing monitor, so could be a fixed calculation 
		float padding_cm = 0.1f;
		int xp = coordinateHelper.CmToPixel((float)x - padding_cm);
		int yp = coordinateHelper.CmToPixel((float)y - padding_cm);
		int wp = coordinateHelper.CmToPixel((float)w + 2.0f * padding_cm);
		int hp = coordinateHelper.CmToPixel((float)h + 2.0f * padding_cm);
		DrawRectangleLines( xp, yp, wp, hp, color);
	}
};


/**
 * Serves as the central hub that integrates various components, 
 * ensuring communication and coordination between them.
 */
class Engine
{
public:
    Engine();
    void Init();
    void UpdateTimeSlice();
    void Render();
    void RandomTestDrawings();
    void Shutdown();
	bool ShouldClose();

    ~Engine();

private:
    Engine_StateManager stateManager;
    Navigator navigator;
	CoordinateHelper coordinateHelper;
	Engine_NetworkDrawingManager networkDrawingManager;
	Canvas canvas;
    Texture wabbit;
	void HandleStateChanges();
	
};

#endif