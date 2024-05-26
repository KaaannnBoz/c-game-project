/**************************************************************************
 * Nom du fichier : attaques.h
 * Description    : Contient toutes les fonbctions principales peremttant de gérére les attaques dans le jeu
 * Auteurs        : Team GEGK
 *                  Inspiré par https://fr.wikipedia.org/wiki/Distance_de_Manhattan pour les distances
 *								https://fr.wikipedia.org/wiki/Distance_de_Tchebychev
 *					contient les fonctions utiles à la gestion des attaques
 **************************************************************************/


#ifndef C_GAME_PROJECT_ATTAQUES_H
#define C_GAME_PROJECT_ATTAQUES_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "player_ia.h"

bool estAttaquePossibleLigneColonne(int porteeMax,int ligneCourante, int colonneCourante,int ligneCible, int colonneCible);
pionGrille* estAttaquePossible(pionGrille* pionQuiAttaque,int ligneCible, int colonneCible,pionGrille pions[]);
bool estAttaquePossiblePion(pionGrille* pionQuiAttaque,pionGrille* pionAttaque);
void attaquerPion(pionGrille* pionQuiAttaque,pionGrille* pionAttaque);
void attaque();

#endif //C_GAME_PROJECT_ATTAQUES_H
