#ifndef GAME_H
#define GAME_H

#include <raylib.h>

#define BOARD_ROWS 6
#define BOARD_COLS 6

typedef enum { PLAYER_ONE, PLAYER_TWO } Player;

typedef struct {
    int row;
    int col;
} Position;

typedef struct {
    Player player;
    Position position;
} GamePiece;

extern GamePiece player1;
extern GamePiece player2;
extern GamePiece *currentPlayer;

extern char mainBoard[BOARD_ROWS][BOARD_COLS];

void InitGame();
void UpdateGame();
void MovePlayer(GamePiece *player, int newRow, int newCol);
void SwitchPlayer();
void EndGame();

#endif
