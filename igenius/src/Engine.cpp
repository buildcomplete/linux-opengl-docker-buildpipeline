#include "Engine.h"
#include "IG_types.h"
#include <cstdio>


Engine::Engine()
{
}

void Engine::Init()
{
	// Load a texture from the resources directory
	wabbit = LoadTexture("wabbit_alpha.png");
 
    // navigator = Navigator();
	// coordinateHelper = CoordinateHelper();
	// stateManager = IG_Engine_StateManager();

     // Define input components
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::UNDEFINED), 1, 1);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::VIRTUAL_CAMERA), 1, 8);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::SP_CONVOLUTION), 4, 7);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::SP_CONVOLUTION), 4, 9);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::IMG_IMG_IMG_OPERATION), 9, 5);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::IMG_IMG_IMG_OPERATION), 9, 9);
}

void Engine::UpdateTimeSlice()
{
    stateManager.UpdateFromInput();
	frameCoords = navigator.SyncWithState(stateManager, coordinateHelper.pixPr_cm);
    HandleStateChanges();

    canvas.Update();
}

void Engine::Render()
{

    // drawing
    BeginDrawing();

    BeginMode2D(navigator.camera);
    {
        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(OLIVE_GREEN);
        
        navigator.DrawCursorWorldGuide(frameCoords, stateManager, canvas);

        RandomTestDrawings();

        canvas.Draw(coordinateHelper, MOUSE_MODE_FLAGS::IG_MOUSE_MODE_NETWORK & stateManager.GetFlags());
        networkDrawingManager.Draw(
            frameCoords,
            IG_MOUSE_MODE_NETWORK & stateManager.GetFlags(),
            coordinateHelper.pixPr_cm);
    }
    EndMode2D();

    navigator.DrawCursorScreenGuide(stateManager);

    // draw some text using the default font
    char buffer[100];

    //sprintf(buffer, "Cell: 0x%02X%02X", cell.x, cell.y );
    const CellPosition& cell = frameCoords.mousePosWorldGrid;
    GridContentInfo info = canvas.GetCellInfo(cell.x, cell.y);
    sprintf(buffer, "Cell: %d,%d:C=%d, N=%d", cell.x, cell.y, info.componentId, info.networkId );
    DrawText(buffer, 20,20,20,WHITE);
    auto component = canvas.GetComponent(info.componentId);
    if (component != nullptr)
    {
        sprintf(buffer, "Component ctx: %d = %d -> %d", component->id, component->bluePrint.inputDataTypes[0].type, (int)(component->bluePrint.outputDataType.type) );
        DrawText(buffer, 20,40,20,WHITE);
    }    
    // end the frame and get ready for the next one  (display frame, poll input, etc...)
    EndDrawing();
}

void Engine::RandomTestDrawings()
{
    // draw our texture to the screen
    DrawTexture(wabbit, 400, 200, WHITE);

    Vector2 points[5] = {
        {-10.0f, -10.0f},
        {-10.0f, 10.0f},
        {10.0f, 10.0f},
        {10.0f, -10.0f},
        {-10.0f, -10.0f}};
    DrawSplineLinear(points, 5, 3, GREEN);
}

void Engine::Shutdown()
{
    // cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

    // destroy the window and cleanup the OpenGL context
	CloseWindow();
}

bool Engine::ShouldClose()
{
    return WindowShouldClose();
}

Engine::~Engine()
{
	
}

void Engine::HandleStateChanges()
{
    if (stateManager.DidEnterState(IG_MOUSE_MODE_NETWORK))
    {
        networkDrawingManager.StartNewNetwork();
    }

    if (stateManager.DidExitState(IG_MOUSE_MODE_NETWORK))
    {
        networkDrawingManager.CompleteDrawing(canvas);
    }

    if (stateManager.DidEnterState(IG_MOUSE_TRY_COMMAND))
    {
        // Try to select component
        if (IG_MOUSE_SELECTING & stateManager.GetFlags())
        {
            // Why do I currently need to be able to select objects?
            // If I should be able to set properties etc dynamically, I should probably add some UI library
        }

        // Send network command, begin new, select and existing, expand network
        if (IG_MOUSE_MODE_NETWORK & stateManager.GetFlags())
        {
            networkDrawingManager.AddAnchorPoint(frameCoords, coordinateHelper.pixPr_cm);
        }
    }
}
