/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "Navigator.h"


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
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		navigator.HandleInput();

		// drawing
		BeginDrawing();

		BeginMode2D(navigator.camera);

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		DrawRectangleLines( 10, 10, 250, 113, BLUE);


		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		// draw circle close to where mouse is
		float res = 50.0f;
		Vector2 worldPos = GetScreenToWorld2D(navigator.gameMousePos, navigator.camera);

		Vector2 quantifiedCenter =  { 
			(float)round(worldPos.x / res) * res,
			(float)round(worldPos.y / res) * res};

		
		for (int r = -2;r<3;++r) {
			for (int c = -2;c<3;++c) {
				Vector2 indicatorPos = {
					quantifiedCenter.x + c*res,
					quantifiedCenter.y + r*res };

				Color circleColor = WHITE;
				float alpha = Clamp(
					1.0f-Vector2Distance(worldPos, indicatorPos) / (2.5f * res), 
					0.0f, 
					1.0f);
				
				circleColor.a = (int)floor(255.0f * alpha);

				DrawCircleLines( indicatorPos.x, indicatorPos.y, 10, circleColor );
			}
		}


		EndMode2D();

		// draw some text using the default font
		DrawText("Hello Raylib", 20,20,20,WHITE);

		
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
