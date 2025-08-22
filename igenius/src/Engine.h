#ifndef IG_ENGINE_H
#define IG_ENGINE_H

#include "Engine_StateManager.h"
#include "Navigator.h"
#include "CoordinateHelper.h"
#include "Canvas.h"
#include "Engine_NetworkDrawingManager.h"
#include "context/NavigationContext.h"
#include "context/RenderContext.h"
#include "context/StateContext.h"


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
    Texture cameraTexture;
	void HandleStateChanges();
	
    StateContext stateContext;
    NavigationContext navigationContext;
	
};

#endif