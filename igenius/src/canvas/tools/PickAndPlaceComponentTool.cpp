#include "PickAndPlaceComponentTool.h"
#include "raylib.h"
#include "commands/MoveComponentCommand.h"

PickAndPlaceComponentTool::PickAndPlaceComponentTool(Canvas &c) : CanvasToolBase(c)
{
}

void PickAndPlaceComponentTool::Draw(const RenderContext &rc, const NavigationContext &navCtx) const
{
    DrawCircle(navCtx.mousePosWorldPixels.x, navCtx.mousePosWorldPixels.y, 5, YELLOW);
    if (pickedComponent)
    {
        DrawRectangle(
            pickedComponent->anchor.x * navCtx.pixPr_cm,
            pickedComponent->anchor.y * navCtx.pixPr_cm,
            pickedComponent->bluePrint.Width * navCtx.pixPr_cm,
            pickedComponent->bluePrint.Height * navCtx.pixPr_cm,
            ColorAlpha(RED, 0.1));
    
        auto last = pickedComponent->anchor;
        pickedComponent->anchor = getAnchorDropPositionGivenOffset(navCtx);
        pickedComponent->Draw(rc,navCtx);
        pickedComponent->anchor=last;
    }
}

std::unique_ptr<ICommand> PickAndPlaceComponentTool::HandleEventsAndTime(const StateContext &stCtx, const NavigationContext &navCtx)
{
    if (!stCtx.DidEnterState(INPUT_STATE_FLAGS::IG_INPUT_TRY_COMMAND))
        return nullptr;

    // Click = with nothing selected = try to pick..
    if ( !pickedComponent)
    {
        pickedComponent = canvas.GetComponent(
            canvas.GetCellInfo(
                navCtx.mousePosWorldGrid).componentId);
        // Calculate mouse offset inside component so component doesnt jump when clicked, 
        // relative mouse pos should be used as grab point.
        pickOffsetCm = pickedComponent->GetCursorOffset(navCtx);
    } 
    // Click = with something selected = try to place
    else
    {
        if (canvas.IsGridFree(
            pickedComponent->bluePrint, 
            navCtx.mousePosWorldGrid.x, 
            navCtx.mousePosWorldGrid.y, 
            pickedComponent->id))
        {
            auto cmd = std::make_unique<MoveComponentCommand>(
                canvas, 
                pickedComponent->id, 
                getAnchorDropPositionGivenOffset(navCtx));
            pickedComponent = nullptr;
            return cmd;
        }
    }
    

    return nullptr;
}

CellPosition PickAndPlaceComponentTool::getAnchorDropPositionGivenOffset(const NavigationContext &navCtx) const
{
    auto wpWithOffset = navCtx.mousePosWorldCm - pickOffsetCm;
    return {(int)round(wpWithOffset.x), (int)round(wpWithOffset.y) };
}
