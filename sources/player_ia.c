#include "player_ia.h"
#include <stdlib.h>
#include <time.h>

void SimulatePlayer(GamePiece *player) {
    // Simuler le déplacement automatique du joueur (contrôlé par l'ordinateur)

    // Générer un mouvement aléatoire pour le joueur
    int randomDirection = rand() % 4; // Génère un nombre aléatoire entre 0 et 3

    switch (randomDirection) {
        case 0: // Déplacement vers la droite
            MovePlayer(player, player->position.row, player->position.col + 1);
            break;
        case 1: // Déplacement vers la gauche
            MovePlayer(player, player->position.row, player->position.col - 1);
            break;
        case 2: // Déplacement vers le bas
            MovePlayer(player, player->position.row + 1, player->position.col);
            break;
        case 3: // Déplacement vers le haut
            MovePlayer(player, player->position.row - 1, player->position.col);
            break;
        default:
            break;
    }
}
