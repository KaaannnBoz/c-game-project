/**************************************************************************
 * Nom du fichier : player_ia.h
 * Description    : Contient les fonctions de simulation des joueurs (IA)
 * Auteurs        : Team GEGK
 **************************************************************************/
 
#ifndef PLAYER_AI_H
#define PLAYER_AI_H

#include "game.h"


// Structure d'un coup d'IA
typedef struct {
    int campPion; // Indique le camp du pion en mouvement
	int pionEnMouvement; // Indique le pion qui va bouger
	int ligneCibleMouvement; // Indique l'endroit où il bouge
	int colonneCibleMouvement; // Indique l'endroit où il bouge
    int pionAttaque; // Indique le pion attaqué
    pionGrille tableauPions[NOMBRE_PIONS_MAX]; // Tableau de pions pour le score/note
    int note; // note du tableau de pions
} coupIA;

void calculCoupIA(coupIA* coupEnCours,pionGrille pions[], int tourActuel);

extern coupIA coupIAEnCours; // Coup en cours de l'IA

#endif
