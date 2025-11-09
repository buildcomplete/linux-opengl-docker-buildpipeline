#include "Navigator.h"
#include "IG_types.h"
#include <iostream>
#include <vector>

Navigator::Navigator()
{
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    gameMousePos = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    DisableCursor();
}

NavigationContext Navigator::HandleEvents(StateContext &sc, float pixPr_cm_)
{
    if (sc.DidEnterState(IG_INPUT_SCREEN_DRAGGING))
    {
        dragAcceleration.x = dragAcceleration.y = 0;
    }

    const Vector2& md = sc.inputCursorDelta;
    if (sc.IsInState(IG_INPUT_SCREEN_DRAGGING))
    {
        dragAcceleration.x -= md.x * 0.1f;
        dragAcceleration.y -= md.y * 0.1f;
        camera.offset.x += dragAcceleration.x;
        camera.offset.y += dragAcceleration.y;

        // Limit drag acceleration speed
        float dragSQ = Vector2LengthSqr(dragAcceleration);
        const float maxDragSQ = 200.0f;
        if (dragSQ > maxDragSQ)
        {
            const float maxDrag = sqrt(maxDragSQ);
            float drag = sqrt(dragSQ);
            dragAcceleration = Vector2Scale(dragAcceleration, maxDrag / drag);
        }
    }
    // If not dragging update mouse positions
    else
    {
        gameMousePos.x += md.x;
        gameMousePos.y += md.y;
    }

    EnforceCameraBounds(pixPr_cm_);

    Vector2 mwp = GetScreenToWorld2D(gameMousePos, camera);
    context = {
        gameMousePos,                                         // mousePosScreenPixel
        mwp,                                                  // mousePosWorldPixels
        {mwp.x / pixPr_cm_, mwp.y / pixPr_cm_},               // mousePosWorldCm,
        {(int)(mwp.x / pixPr_cm_), (int)(mwp.y / pixPr_cm_)}, // CellPosition mousePosWorldGrid
        camera,                                               // Camera2D camera
        pixPr_cm_                                             // float pixPr_cm
    };
    return context;
}

void Navigator::EnforceCameraBounds(float pixPr_cm_)
{
    // Limit camera bound to be within world bounds in cell coords
    Vector2 topLeftWC = NavigationContext::ScreenToWPCMStatic(
        {0, 0}, camera, pixPr_cm_);
    Vector2 bottomRightWC = NavigationContext::ScreenToWPCMStatic(
        {(float)GetScreenWidth(), (float)GetScreenHeight()}, camera, pixPr_cm_);

    if (topLeftWC.x < cameraBoundsWC.x)
    {
        camera.offset.x = -cameraBoundsWC.x * pixPr_cm_;
    }
    if (topLeftWC.y < cameraBoundsWC.y)
    {
        camera.offset.y = -cameraBoundsWC.y * pixPr_cm_;
    }
    if (bottomRightWC.x > cameraBoundsWC.width)
    {
        int overShoot = cameraBoundsWC.width - bottomRightWC.x;
        camera.offset.x -= overShoot * pixPr_cm_;
    }
    if (bottomRightWC.y > cameraBoundsWC.height)
    {
        int overShoot = cameraBoundsWC.height - bottomRightWC.y;
        camera.offset.y -= overShoot * pixPr_cm_;
    }
}

// Should be called while drawing in camera mode
void Navigator::DrawCursorWorldGuide(const StateContext &sc, const Canvas &canvas) const
{
    // Assuming drawing in camera mode.
    // draw circle close to where mouse is
    Vector2 worldPos = context.mousePosWorldPixels;
    CellPosition cPos = context.mousePosWorldGrid;
    Vector2 quantifiedCenter = {cPos.x * context.pixPr_cm, cPos.y * context.pixPr_cm};

    if (sc.IsInState(IG_INPUT_MODE_SELECTING))
    {
        for (int r = -3; r < 4; ++r)
        {
            for (int c = -3; c < 4; ++c)
            {

                // Determine color depending in wheter or not we are overlapping with component or network
                Color circleColor = (canvas.GetCellInfo(cPos.x + c, cPos.y + r).componentId == 0) ? WHITE : RED;
                Vector2 indicatorPos = {
                    quantifiedCenter.x + c * context.pixPr_cm,
                    quantifiedCenter.y + r * context.pixPr_cm};

                // the quantified center is the upper left vertic, we need to calculate distance as we are in the center of the vertex.
                float alpha = Clamp(
                    1.0f - Vector2Distance(Vector2AddValue(indicatorPos, 0.5f * context.pixPr_cm), worldPos) / (2.2f * context.pixPr_cm),
                    0.0f,
                    1.0f);
                if (alpha > 0.01)
                {
                    float marginF = Clamp(
                        1.0f - Vector2Distance(Vector2AddValue(indicatorPos, 0.5f * context.pixPr_cm), worldPos) / (2.5f * context.pixPr_cm),
                        0.2f,
                        1.0f);
                    int margin = marginF * context.pixPr_cm / 5;

                    circleColor.a = (int)floor(255.0f * alpha);
                    DrawRectangleLines((int)indicatorPos.x + margin, (int)indicatorPos.y + margin, (int)context.pixPr_cm - 2 * margin, (int)context.pixPr_cm - 2 * margin, circleColor);
                }
            }
        }
    }
}

// Should be called while drawing in fixed screen coordinates (Outhside camera mode)
void Navigator::DrawCursorScreenGuide(const StateContext &sc) const
{
    // Draw cross where we are, color based on input mode
    auto drawCrossAtCursor = [this](Color C)
    {
        DrawLine(
            gameMousePos.x - 10, gameMousePos.y,
            gameMousePos.x + 10, gameMousePos.y,
            C);
        DrawLine(
            gameMousePos.x, gameMousePos.y - 10,
            gameMousePos.x, gameMousePos.y + 10,
            C);
    };

    auto getCursorColor = [sc]()
    {
        return  
            sc.IsInState(IG_INPUT_MODE_SELECTING) ? NETGREEN :
            sc.IsInState(IG_INPUT_MODE_DRAW_NETWORK) ? ELECTRIC_BLUE :
            sc.IsInState(IG_INPUT_MODE_DELETE) ? RED :
            sc.IsInState(IG_INPUT_MODE_PLACE_COMPONENT) ? BLUE :
            sc.IsInState(IG_INPUT_MODE_MOVE_COMPONENT) ? PURPLE :
            PINK;
    };

    // get cursor color, this construct ensures the conditions are only evaluated when any flags was changed.
    static auto cursorColor = getCursorColor();
    if (sc.flippedFlags)
        cursorColor = getCursorColor();

    drawCrossAtCursor(cursorColor);
    if (sc.IsInState(IG_INPUT_SCREEN_DRAGGING))
    {
        DrawLine(
            gameMousePos.x - dragAcceleration.x * 1.5, gameMousePos.y - dragAcceleration.y * 1.5,
            gameMousePos.x - dragAcceleration.x * 5, gameMousePos.y - dragAcceleration.y * 5,
            cursorColor);
    }
}
