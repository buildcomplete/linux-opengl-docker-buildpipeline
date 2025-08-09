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

	Rectangle player = { 400, 280, 40, 40 };
	Camera2D camera = { 0 };
    camera.target = (Vector2){ player.x + 20.0f, player.y + 20.0f };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
	
	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
	
	bool isDragging = false;
	bool wasDraggin = false;
	Vector2 dragAcceleration = {0,0};
	Vector2 gameMousePos = {0,0}; // Hack to store mouse pos between enabling end disabling mouse drag
	bool skipUpdateMousePos = false;
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{

        // Camera target follows player
        camera.target = (Vector2){ player.x + 20, player.y + 20 };

        // Camera rotation controls
        if (IsKeyDown(KEY_A)) camera.rotation--;
        else if (IsKeyDown(KEY_S)) camera.rotation++;

		isDragging = IsMouseButtonDown(1);
		if (isDragging != wasDraggin)
		{
			wasDraggin=isDragging;
			dragAcceleration.x = dragAcceleration.y = 0;
			if (isDragging)
			{
				DisableCursor();
			}
			else
			{
				EnableCursor();
				SetMousePosition(gameMousePos.x, gameMousePos.y);
				skipUpdateMousePos=true;
			}
		}

		if (isDragging)
		{
			Vector2 md = GetMouseDelta();
			dragAcceleration.x -= md.x * 0.1f;
			dragAcceleration.y -= md.y * 0.1f;
			camera.offset.x += dragAcceleration.x;
			camera.offset.y += dragAcceleration.y;
		}
		else if (!skipUpdateMousePos)
		{
			gameMousePos = GetMousePosition();
		}
		skipUpdateMousePos=false;

		// drawing
		BeginDrawing();

		BeginMode2D(camera);

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		DrawRectangleLines( 10, 10, 250, 113, BLUE);

		// draw some text using the default font
		DrawText("Hello Raylib", 20,20,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		// draw circle close to where mouse is
		float res = 50.0f;
		Vector2 worldPos = GetScreenToWorld2D(gameMousePos, camera);

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
