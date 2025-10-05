/*
IG Host loop
*/

#include "raylib.h"
#include "Engine.h"
#include <resource_dir.h>

void InitRaylibWindow()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI );
	//SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	SetWindowMonitor(1);
	int screenWidth = GetMonitorWidth(1), screenHeight = GetMonitorHeight(1);
	//int screenWidth = 640, screenHeight = 480;

	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "I**G***** - Core");
	
	screenWidth = GetMonitorWidth(1), screenHeight = GetMonitorHeight(1);
	//SetWindowPosition(100, 100);
	SetWindowMonitor(1);
	//ToggleFullscreen();

	SetTargetFPS(60); // Set our game to run at 60 frames-per-second

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
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
		engine.HandleEvents();
		engine.Render();
	}

	engine.Shutdown();

	return 0;
}
