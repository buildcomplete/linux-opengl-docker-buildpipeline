#pragma once

#include "Engine_StateManager.h"
#include "Navigator.h"
#include "CoordinateHelper.h"
#include "canvas/Canvas.h"
#include "canvas/tools/DrawNetworkTool.h"
#include "context/NavigationContext.h"
#include "context/RenderContext.h"
#include "context/StateContext.h"
#include "canvas/tools/ToolMap.h"
#include "commands/CommandStack.h"


/**
 * Serves as the central hub that integrates various components, 
 * ensuring communication and coordination between them.
 */
class Engine
{
public:
    Engine();
    void Init();
    void HandleEvents();
    void Render();
    void RandomTestDrawings();
    void Shutdown();
	bool ShouldClose();

    ~Engine();

private:
    Engine_StateManager stateManager;
    Navigator navigator;
	CoordinateHelper coordinateHelper;
	Canvas canvas;
    Texture cameraTexture;
	void InjectStateChanges();
	
    StateContext stateContext;
    NavigationContext navigationContext;
    std::unique_ptr<ToolMap> toolMap;
    CanvasToolBase* activeTool = nullptr;
    CommandStack commandStack;
	
};
