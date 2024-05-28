/**************************************************************************
 * Nom du fichier : deplacements.h
 * Description    : Contient toutes les fonctions principales permettant de gerer les déplacements des pions dans le jeu
 * Auteurs        : Team GEGK
 *                  Inspiré par https://fr.wikipedia.org/wiki/Distance_de_Manhattan pour les distances
 *								https://fr.wikipedia.org/wiki/Distance_de_Tchebychev
 **************************************************************************/
 
#ifndef C_GAME_PROJECT_DEPLACEMENTS_H
#define C_GAME_PROJECT_DEPLACEMENTS_H

#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include "game.h"
#include "player_ia.h"

bool estDeplacementPossibleLigneColonne(int deplacementMax,int ligneCourante, int colonneCourante,int ligneCible, int colonneCible);
bool estCheminLibre(const pionGrille *pion, int ligneCible, int colonneCible,pionGrille pions[]);
bool estDeplacementPossible(const pionGrille *pion, int ligneCible, int colonneCible,pionGrille pions[]);
void deplacerPion(pionGrille *pion, int ligneCible, int colonneCible);
void deplacement();
void resetPionsFatigues(pionGrille pions[]);

#endif //C_GAME_PROJECT_DEPLACEMENTS_H
