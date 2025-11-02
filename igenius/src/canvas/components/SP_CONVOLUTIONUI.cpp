#include "SP_CONVOLUTIONUI.h"
#include <iostream>

CanvasComponents::SP_CONVOLUTIONUI::SP_CONVOLUTIONUI(std::uint8_t id_, CellPosition anchor_, const ComponentBluePrint &bluePrint_)
    : CanvasComponentBase(id_, anchor_, bluePrint_), 
      menu([this](int idx, int opId) { OnMenuSelectOperationClick(idx); }) 
    {}

void CanvasComponents::SP_CONVOLUTIONUI::Draw(const RenderContext &rc, const NavigationContext &navCtx) const
{
    const float padding_cm = 0.1f;
    int xp = navCtx.CmToPixel(anchor.x + padding_cm);
    int yp = navCtx.CmToPixel(anchor.y + 0.5f);
    int wp = navCtx.CmToPixel(bluePrint.Width - 2.0f * padding_cm);
    int hp = navCtx.CmToPixel(2);
    DrawRectangleLines(xp + 1, yp + 1, wp - 2, hp - 2, ELECTRIC_BLUE);

    // Draw sockets on the edge as circles
    // In theory the sockets could be on any locations, but for simplicity we add them here
    // The socket shape depends on the type
    // The socket color changes with socket number

    // Input sockets on the left,
    float ys0 = yp + hp / 2.0f;
    float inPosX = xp;
    DrawCircleLinesV({inPosX, ys0}, navCtx.pixPr_cm / 8.0f, WHITE);

    // output on the right
    if (bluePrint.outputDataType.type != DT_NONE)
    {
        float y_out = navCtx.CmToPixel((float)anchor.y + (float)bluePrint.Height / 2.0f);
        float outPosX = xp + wp;
        DrawCircleLinesV({outPosX, y_out}, navCtx.pixPr_cm / 8.0f, WHITE);
    }

    // At the center of the components, draw convolution widget that both show the current signal,
    // and shows the shape of the current signal in the center
    // We should be able to reach on click on the selector for selecting signal,
    // And to listen for click on direction selectors
    float cx = navCtx.CmToPixel((float)anchor.x + (float)bluePrint.Width / 2.0f);
    float cy = navCtx.CmToPixel((float)anchor.y + (float)bluePrint.Height / 2.0f);

    float t = Wrap(GetTime(), 0.0f, 4.0f) * 90;
    // rotate every 4 second;

    DrawCircleSelectorLines({cx, cy}, navCtx.pixPr_cm * 0.75f, 90 + t, 270 + t, 1, WHITE);
    //menu.Draw(rc);
}
bool CanvasComponents::SP_CONVOLUTIONUI::TryStartCommand(const StateContext &stCtx, const NavigationContext &navCtx)
{
    if (!menu.IsVisible())
    {
        menu.Show({anchor.x + bluePrint.Width/2.0f, anchor.y + bluePrint.Height/2.0f} );
    }
    return true;
}

bool CanvasComponents::SP_CONVOLUTIONUI::HandleEventsWhileFocused(const StateContext &stCtx, const NavigationContext &navCtx)
{
    menu.HandleEventsAndTime(stCtx, navCtx);

    return menu.IsVisible();
}

void CanvasComponents::SP_CONVOLUTIONUI::FocusedDraw(const RenderContext &rc, const NavigationContext &navCtx) const
{
    menu.Draw(rc, navCtx);
}

void CanvasComponents::SP_CONVOLUTIONUI::OnMenuSelectOperationClick(int segmentIndex)
{
    std::cout << "** void UI_Components::SP_CONVOLUTIONUI::OnMenuSelectOperationClick(int segmentIndex= " << segmentIndex << ")" << std::endl;

    menu.Hide();
}
