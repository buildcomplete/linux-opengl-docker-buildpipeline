#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H
#include <raylib.h>

// lightweight non-owning context
struct RenderContext
{
    float pixPr_cm = 1;
    Texture &cameraTexture;
    float CmToPixel(float cm) const {return cm*pixPr_cm;} 

    // // optional helpers/wrappers for C API:
    // void DrawRectangle(int x, int y, int w, int h, Color c) const {
    //     ::DrawRectangle(x,y,w,h,c);
    // }
    // void DrawTexturePro(const Texture& tex, Rect src, Rect dst, Vec2 origin, float rot, Color tint) const {
    //     ::DrawTexturePro(tex, src, dst, origin, rot, tint);
    // }
};


#endif