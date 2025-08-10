/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <iostream>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "Navigator.h"
#include "CoordinateHelper.h"
#include "ig_types.h"

class UIBoxComponent
{
public:	
	
	int x,y,h,w; // Grid aligned (cm) base coordinates
	Color color;

	UIBoxComponent(int _x, int _y, int _h, int _w, Color _color) 
		: x(_x), y(_y), h(_h),w(_w), color(_color)
	{
		
	}

	void Draw(CoordinateHelper& coordinateHelper)
	{
		// This is only updated when changing monitor, so could be a fixed calculation 
		float padding_cm = 0.1f;
		int xp = coordinateHelper.CmToPixel((float)x - padding_cm);
		int yp = coordinateHelper.CmToPixel((float)y - padding_cm);
		int wp = coordinateHelper.CmToPixel((float)w + 2.0f * padding_cm);
		int hp = coordinateHelper.CmToPixel((float)h + 2.0f * padding_cm);
		DrawRectangleLines( xp, yp, wp, hp, color);
	}
};

int main ()
{

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI );

	int screenWidth = 800, screenHeight = 600;

	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
	
	Navigator navigator;
	CoordinateHelper coordinateHelper;

	// Define all input frames
	const int numComponents = 6;
	UIBoxComponent components[numComponents] = 
	{
		UIBoxComponent(1,1,1,1, ELECTRIC_BLUE), // Test component is 1x1, starting at 1x1
		UIBoxComponent(1,10,2,2, ELECTRIC_BLUE), // Camera is 2x2, starting at 1x10
		UIBoxComponent(4,6,2,3, ELECTRIC_BLUE), // ConvolveHorz is 2x3, starting at 4x7
		UIBoxComponent(4,9,2,3, ELECTRIC_BLUE), // ConvolveHorz is 2x3, starting at 4x9
		UIBoxComponent(9,6,2,2, ELECTRIC_BLUE), // SquareConvHorz is 2x2, starting at 9x7
		UIBoxComponent(9,9,2,2, ELECTRIC_BLUE) // SquareConvolveHorz is 2x4, starting at 9x9
	};
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		navigator.HandleInput();

		// drawing
		BeginDrawing();

		BeginMode2D(navigator.camera);
		{
			navigator.DrawCursorWorldGuide(coordinateHelper.pixPr_cm);

			// Setup the back buffer for drawing (clear color and depth buffers)
			ClearBackground(OLIVE_GREEN);

			// draw our texture to the screen
			DrawTexture(wabbit, 400, 200, WHITE);

			Vector2 points[5] = {
				{-10.0f, -10.0f },
				{-10.0f,  10.0f },
				{ 10.0f,  10.0f },
				{ 10.0f,  -10.0f},
				{-10.0f, -10.0f }

			};
			DrawSplineLinear(points, 5, 3, GREEN);

			for (int i=0;i<numComponents;++i)
				components[i].Draw(coordinateHelper);

		}
		EndMode2D();

		navigator.DrawCursorScreenGuide();

		// draw some text using the default font
		char buffer[100];
		sprintf(buffer, "Image Genius: %.2f", (float)GetRandomValue(0, 100)/100.0f);
		DrawText(buffer, 20,20,20,WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
		
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
