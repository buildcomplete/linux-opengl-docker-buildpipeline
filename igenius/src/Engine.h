#ifndef IG_ENGINE_H
#define IG_ENGINE_H

#include "Engine_StateManager.h"
#include "Navigator.h"
#include "CoordinateHelper.h"
#include "Canvas.h"
#include "Engine_NetworkDrawingManager.h"

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
	IG_MouseCoordinates frameCoords;
	Canvas canvas;
    Texture wabbit;
	void HandleStateChanges();
	
};

#endif