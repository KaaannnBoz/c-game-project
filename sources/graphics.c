#include "graphics.h"
#include "game.h"
#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SQUARE_SIZE 80

void DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Dessiner le damier
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            Rectangle square = { j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
            DrawRectangleRec(square, BLACK);

            // Dessiner les joueurs sur le damier
            if (player1.position.row == i && player1.position.col == j) {
                DrawText("P1", square.x + 10, square.y + 10, 20, RED);
            }
            if (player2.position.row == i && player2.position.col == j) {
                DrawText("P2", square.x + 10, square.y + 10, 20, BLUE);
            }
        }
    }

    EndDrawing();
}