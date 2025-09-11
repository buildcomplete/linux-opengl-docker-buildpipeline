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
}

void Engine::HandleEvents()
{
    stateContext = stateManager.HandleEvents();
    navigationContext = navigator.HandleEvents(stateContext, coordinateHelper.pixPr_cm);

    auto c = stateManager.GetFocusComponent();
    if (c != nullptr)
    {
        bool keepFocus = c->HandleEventsWhileFocused(stateContext, navigationContext);
        if (!keepFocus)
        {
            stateManager.SetFocusComponent(nullptr);
        }
    }

    InjectStateChanges();
}

void Engine::Render()
{
    // Get context objects;
    RenderContext rc = {
        coordinateHelper.pixPr_cm,
        cameraTexture};
    // drawing
    BeginDrawing();

    BeginMode2D(navigator.camera);
    {
        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(OLIVE_GREEN);

        navigator.DrawCursorWorldGuide(stateContext, canvas);

        RandomTestDrawings();

        canvas.Draw(navigationContext, rc, stateContext);
        networkDrawingManager.Draw(navigationContext, rc, stateContext);

        auto focusedComponent = stateManager.GetFocusComponent();
        if (focusedComponent != nullptr)
        {
            stateManager.GetFocusComponent()->FocusedDraw(rc);
        }
    }
    EndMode2D();

    navigator.DrawCursorScreenGuide(stateContext);

    // draw some text using the default font
    static char buffer[100];
    int debugLine = 1;

    const CellPosition &cell = navigationContext.mousePosWorldGrid;
    GridContentInfo info = canvas.GetCellInfo(cell.x, cell.y);
    sprintf(buffer, "Cell: %d,%d:C=%d, N=%d", cell.x, cell.y, info.componentId, info.networkId);
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
        auto topLeftWC = GetScreenToWorld2D({0, 0}, navigator.camera);
        auto bottomRightWC = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, navigator.camera);
        int 
            topCellX = (int)round(topLeftWC.x / rc.pixPr_cm),
            topCellY = (int)round(topLeftWC.y / rc.pixPr_cm),
            bottomCellX = (int)round(bottomRightWC.x / rc.pixPr_cm),
            bottomCellY = (int)round(bottomRightWC.y / rc.pixPr_cm);

        sprintf(buffer, "Visible bounds: (%d,%d)-(%d,%d)", topCellX, topCellY, bottomCellX, bottomCellY);
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
    if (stateContext.DidEnterState(IG_MOUSE_MODE_NETWORK))
    {
        networkDrawingManager.StartDrawing();
    }

    if (stateContext.DidExitState(IG_MOUSE_MODE_NETWORK))
    {
        networkDrawingManager.CompleteDrawing(canvas);
    }

    // Send network command, begin new, select and existing, expand network

    if (stateContext.DidEnterState(IG_MOUSE_TRY_COMMAND))
    {
        if (stateContext.IsInState(IG_MOUSE_SELECTING))
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
                {
                    auto c = canvas.GetComponent(info.componentId);
                    if (c->TryStartCommand(stateContext, navigationContext))
                    {
                        stateManager.SetFocusComponent(c);
                    }
                }
            }
        }
        if (stateContext.IsInState(IG_MOUSE_MODE_NETWORK))
        {
            networkDrawingManager.AddAnchorPoint(navigationContext, coordinateHelper.pixPr_cm);
        }
    }
}
