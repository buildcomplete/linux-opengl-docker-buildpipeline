#include "Engine.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "IG_types.h"
#include <cstdio>


Engine::Engine()
{
}

void Engine::Init()
{
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	wabbit = LoadTexture("wabbit_alpha.png");
 
    // navigator = Navigator();
	// coordinateHelper = CoordinateHelper();
	// stateManager = IG_Engine_StateManager();
}

void Engine::UpdateTimeSlice()
{
    stateManager.UpdateFromInput();
	navigator.SyncWithState(stateManager, coordinateHelper.pixPr_cm);
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
        
        navigator.DrawCursorWorldGuide(stateManager, coordinateHelper.pixPr_cm);

        RandomTestDrawings();

        canvas.Draw(coordinateHelper);
        networkDrawingManager.Draw(
            navigator.GetMousePosWorld(),
            IG_MOUSE_MODE_NETWORK & stateManager.GetFlags(),
            coordinateHelper.pixPr_cm);
    }
    EndMode2D();

    navigator.DrawCursorScreenGuide(stateManager);

    // draw some text using the default font
    char buffer[100];
    sprintf(buffer, "Image Genius: %.2f", (float)GetRandomValue(0, 100)/100.0f);
    DrawText(buffer, 20,20,20,WHITE);
    
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
        networkDrawingManager.CompleteDrawing();
    }

    if (stateManager.DidEnterState(IG_MOUSE_TRY_COMMAND))
    {
        // Try to select component
        if (IG_MOUSE_SELECTING & stateManager.GetFlags())
        {

        }

        // Send network command, begin new, select and existing, expand network
        if (IG_MOUSE_MODE_NETWORK & stateManager.GetFlags())
        {
            networkDrawingManager.AddAnchorPoint(navigator.GetMousePosWorld(), coordinateHelper.pixPr_cm);
        }
    }
}
