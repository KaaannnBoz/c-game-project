#include "player_ia.h"
#include <stdlib.h>
#include <time.h>

void SimulatePlayer(pionGrille *pion) {
    // Simuler le déplacement automatique du joueur (contrôlé par l'ordinateur)

    // Générer un mouvement aléatoire pour le joueur
    int randomDirection = rand() % 4; // Génère un nombre aléatoire entre 0 et 3

    switch (randomDirection) {
        case 0: // Déplacement vers la droite
            deplacerPion(pion,pion->positionLigne, pion->positionColonne + 1);
            break;
        case 1: // Déplacement vers la gauche
            deplacerPion(pion,pion->positionLigne, pion->positionColonne - 1);
            break;
        case 2: // Déplacement vers le bas
            deplacerPion(pion,pion->positionLigne+1, pion->positionColonne);
            break;
        case 3: // Déplacement vers le haut
            deplacerPion(pion,pion->positionLigne-1, pion->positionColonne);
            break;
        default:
            break;
    }
}
