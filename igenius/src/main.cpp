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

int main ()
{

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	int screenWidth = 1280, screenHeight = 800;

	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
	
	Navigator navigator;
	CoordinateHelper coordinateHelper;
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		navigator.HandleInput();

		// drawing
		BeginDrawing();

		BeginMode2D(navigator.camera);
		coordinateHelper.Draw(navigator.gameMousePos, navigator.camera);

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		DrawRectangleLines( 10, 10, 250, 113, BLUE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		EndMode2D();

		// draw some text using the default font
		char buffer[100];
		sprintf(buffer, "Hello raylib: %.2f", (float)GetRandomValue(0, 100)/100.0f);
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
