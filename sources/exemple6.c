/*******************************************************************************************
*
*   raylib [core] example - Custom logging
*
*   Example originally created with raylib 2.5, last time updated with raylib 2.5
*
*   Example contributed by Pablo Marcos Oltra (@pamarcos) and reviewed by Ramon Santamaria (@raysan5)
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2018-2024 Pablo Marcos Oltra (@pamarcos) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <stdio.h>                  // Required for: fopen(), fclose(), fputc(), fwrite(), printf(), fprintf(), funopen()

// Custom logging function
// Custom logging funtion
void LogColored(int msgType, const char *text, va_list args)
{
	switch (msgType)
	{
		case LOG_INFO: printf("[\e[1;32mINFO\e[0m] : "); break;
		case LOG_ERROR: printf("[\e[1;31mERROR\e[0m]: "); break;
		case LOG_WARNING: printf("[\e[1;33mWARN\e[0m] : "); break;
		case LOG_DEBUG: printf("[DEBUG]: "); break;
		default: break;
	}

	vprintf(text, args);
	printf("\n");
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int test6(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    // Set custom logger
	SetTraceLogCallback(LogColored);

    InitWindow(screenWidth, screenHeight, "raylib [core] example - custom logging");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Check out the console output to see the custom logger in action!", 60, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    TraceLog(LOG_ERROR, "LOG GALDRIC");
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
