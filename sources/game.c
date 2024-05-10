#include "game.h"
#include "player_ia.h"
#include <stdio.h>

GamePiece player1;
GamePiece player2;
GamePiece *currentPlayer;

char mainBoard[BOARD_ROWS][BOARD_COLS];

void InitGame() {
    // Initialisation du jeu
    player1.player = PLAYER_ONE;
    player1.position.row = 0;
    player1.position.col = 0;

    player2.player = PLAYER_TWO;
    player2.position.row = BOARD_ROWS - 1;
    player2.position.col = BOARD_COLS - 1;

    currentPlayer = &player1;

    // Initialisation du damier
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            mainBoard[i][j] = ' '; // Espace vide par défaut
        }
    }
}

void MovePlayer(GamePiece *player, int newRow, int newCol) {
    // Vérifier si la nouvelle position est valide
    if (newRow >= 0 && newRow < BOARD_ROWS && newCol >= 0 && newCol < BOARD_COLS) {
        // Mettre à jour la position du joueur
        player->position.row = newRow;
        player->position.col = newCol;
    } else {
        printf("Déplacement impossible : position invalide.\n");
    }
}

void SwitchPlayer() {
    // Changer de joueur après chaque tour
    if (currentPlayer == &player1)
        currentPlayer = &player2;
    else
        currentPlayer = &player1;
}

void UpdateGame() {
    // Logique de mise à jour du jeu

    // Gestion de la simulation du joueur 2 (ordinateur) si nécessaire
    if (currentPlayer == &player2) {
        SimulatePlayer(currentPlayer);
    }
}
void EndGame() {
    // Exemple de tâche de nettoyage
    printf("Nettoyage du jeu...\n");

    // Ici, vous pourriez libérer des ressources, fermer des fichiers ou effectuer d'autres opérations de nettoyage si nécessaire
}