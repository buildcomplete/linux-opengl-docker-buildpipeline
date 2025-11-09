#include "Engine.h"
#include "IG_types.h"
#include <cstdio>

Engine::Engine()
{
}

void Engine::Init()
{
    // Load a texture from the resources directory
    cameraTexture = LoadTexture("game_camera_transparent2.png");

    // Define input components
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::UNDEFINED), 1, 1);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::VIRTUAL_CAMERA), 1, 8);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::SP_CONVOLUTION), 4, 7);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::SP_CONVOLUTION), 4, 9);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::IMG_IMG_IMG_OPERATION), 10, 5);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::IMG_IMG_IMG_OPERATION), 10, 9);
    canvas.AddComponent(ComponentFactory::GetBluePrint(CMPNAMES::DRAWING_COMPONENT), 2, 1);


    // Create all tools for canvas
    toolMap = std::make_unique<ToolMap>(canvas);
}

void Engine::HandleEvents()
{
    stateContext = stateManager.HandleEvents();
    navigationContext = navigator.HandleEvents(stateContext, coordinateHelper.pixPr_cm);
    stateManager.UpdateMenus(navigationContext);

    if (activeTool)
    {
        auto cmd = activeTool->HandleEventsAndTime(stateContext, navigationContext);
        if (cmd)
        {
           cmd->Execute(); 
        }
    }
    InjectStateChanges();
}

void Engine::Render()
{
    // Get context objects;
    RenderContext rc = {cameraTexture};
    // drawing
    BeginDrawing();

    BeginMode2D(navigator.camera);
    {
        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(OLIVE_GREEN);

        navigator.DrawCursorWorldGuide(stateContext, canvas);

        canvas.Draw(rc, navigationContext, stateContext);
        //networkDrawingManager.Draw(rc, navigationContext);

        stateManager.Draw(rc, navigationContext);

        if (activeTool)
        {
            activeTool->Draw(rc, navigationContext);
        }
    }
    EndMode2D();

    canvas.DrawOverlay(rc, navigationContext);
    navigator.DrawCursorScreenGuide(stateContext);

    // draw some text using the default font
    static char buffer[100];
    int debugLine = 1;

    const CellPosition &cell = navigationContext.mousePosWorldGrid;
    GridContentInfo info = canvas.GetCellInfo(cell.x, cell.y);
    sprintf(buffer, "Cell: %02X,%02X:C=%02X, N=%02X", cell.x, cell.y, info.componentId, info.networkId);
    DrawText(buffer, 20, 20 * debugLine++, 20, WHITE);

    auto component = canvas.GetComponent(info.componentId);
    if (component != nullptr)
    {
        int inputDT = component->bluePrint.inputDataTypes.size() == 0 ? 0 : component->bluePrint.inputDataTypes[0].type;
        sprintf(buffer, "Component id: %02x = %d -> %d", component->id, inputDT, (int)(component->bluePrint.outputDataType.type));
        DrawText(buffer, 20, 20 * debugLine++, 20, WHITE);
    }

    // Calculate and camera bounds in cell coords
    {
        auto topLeftCI = navigationContext.ScreenToCellIndex({0,0});
        auto bottomRightWC = navigationContext.ScreenToCellIndex({(float)GetScreenWidth(), (float)GetScreenHeight()});
        sprintf(buffer, "Visible bounds: (%d,%d)-(%d,%d)", topLeftCI.x, topLeftCI.y, bottomRightWC.x, bottomRightWC.y);
        DrawText(buffer, 20, 20 * debugLine++, 20, WHITE);
    }
    
    // end the frame and get ready for the next one  (display frame, poll input, etc...)
    EndDrawing();
}

void Engine::RandomTestDrawings()
{
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
    UnloadTexture(cameraTexture);

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

void Engine::InjectStateChanges()
{
    // Starting selecting mode, clear current tool
    if (stateContext.DidEnterState(INPUT_STATE_FLAGS::IG_INPUT_SELECT_MODE))
    {
        activeTool = nullptr;
    }

    // if we are done selecting a mode, check if we set a tool
    if (stateContext.DidExitState(INPUT_STATE_FLAGS::IG_INPUT_SELECT_MODE))
    {
        activeTool = toolMap->GetTool(stateContext.flags);
    }

    // Send network command, begin new, select and existing, expand network
    if (stateContext.DidEnterState(IG_INPUT_TRY_COMMAND))
    {
        if (stateContext.IsInState(IG_INPUT_MODE_SELECTING))
        {
            // // Try to select component
            // if (IG_MOUSE_SELECTING & stateManager.GetFlags())
            // {
            //     // Why do I currently need to be able to select objects?
            //     // If I should be able to set properties etc dynamically, I should probably add some UI library
            // }
            auto info = canvas.GetCellInfo(navigationContext.mousePosWorldGrid);
            if (info.componentId != 0)
            {
                auto c = canvas.GetComponent(info.componentId);
                if (c->TryStartCommand(stateContext, navigationContext))
                {
                    stateManager.SetFocusComponent(c);
                }                
            }
        }
       
    }
}
