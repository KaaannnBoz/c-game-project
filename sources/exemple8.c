#include "raylib.h"

int test8(void)
{
    // Initialisation de la fenêtre
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Incrémenter/Décrémenter Nombre");

    int count = 0;
    Rectangle incrementButton = { screenWidth / 2 - 110, screenHeight / 2, 100, 50 };
    Rectangle decrementButton = { screenWidth / 2 + 10, screenHeight / 2, 100, 50 };

    SetTargetFPS(60); // Fixer le nombre d'images par seconde

    // Boucle principale
    while (!WindowShouldClose())
    {
        // Mise à jour
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePoint = GetMousePosition();

            if (CheckCollisionPointRec(mousePoint, incrementButton))
            {
                count++;
            }
            else if (CheckCollisionPointRec(mousePoint, decrementButton))
            {
                count--;
            }
        }

        // Dessin
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Nombre: %i", count), screenWidth / 2 - 40, screenHeight / 2 - 60, 20, BLACK);

        DrawRectangleRec(incrementButton, LIGHTGRAY);
        DrawText("+", incrementButton.x + 10, incrementButton.y + 15, 20, BLACK);

        DrawRectangleRec(decrementButton, LIGHTGRAY);
        DrawText("-", decrementButton.x + 10, decrementButton.y + 15, 20, BLACK);

        EndDrawing();
    }

    // Déinitialisation
    CloseWindow();

    return 0;
}
