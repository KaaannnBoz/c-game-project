#ifndef C_GAME_PROJECT_ATTAQUES_H
#define C_GAME_PROJECT_ATTAQUES_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

bool estAttaquePossibleLigneColonne(int porteeMax,int ligneCourante, int colonneCourante,int ligneCible, int colonneCible);
pionGrille* estAttaquePossible(int ligneCible, int colonneCible,pionGrille pions[]);
void attaquerPion(pionGrille* pionQuiAttaque,pionGrille* pionAttaque);
void attaque();

#endif //C_GAME_PROJECT_ATTAQUES_H
