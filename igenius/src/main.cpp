/*
IG Host loop
*/

#include "raylib.h"
#include "Engine.h"

void InitRaylibWindow()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI );

	int screenWidth = 800, screenHeight = 600;

	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "I**G***** - Core");

	SetTargetFPS(60); // Set our game to run at 60 frames-per-second
}

int main ()
{
	// Since I try to avoid using pointers, 
	// I need to do some pre-initialization before Contructing instances
	// Or I would need to make a contruct/init procedure
	// (Ideally I would like this code to be in engine init... but then all memebers would need to be pointers since contructor is called on declaration)
 	InitRaylibWindow();

	Engine engine;
	engine.Init();
	
	// game loop
	while (!engine.ShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		engine.UpdateTimeSlice();
		engine.Render();
	}

	engine.Shutdown();

	return 0;
}
