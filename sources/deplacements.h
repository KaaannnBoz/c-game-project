#ifndef C_GAME_PROJECT_DEPLACEMENTS_H
#define C_GAME_PROJECT_DEPLACEMENTS_H

#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include "game.h"

bool estDeplacementPossibleLigneColonne(int deplacementMax,int ligneCourante, int colonneCourante,int ligneCible, int colonneCible);
bool estCheminLibre(const pionGrille *pion, int ligneCible, int colonneCible,pionGrille pions[]);
bool estDeplacementPossible(const pionGrille *pion, int ligneCible, int colonneCible,pionGrille pions[]);
void deplacerPion(pionGrille *pion, int ligneCible, int colonneCible);
void deplacement();

#endif //C_GAME_PROJECT_DEPLACEMENTS_H
