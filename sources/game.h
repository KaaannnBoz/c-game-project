#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

#define NOMBRE_COLONNES_GRILLE 7
#define NOMBRE_LIGNES_GRILLE 5
#define TAILLE_CELLULE_GRILLE 80
#define DECALAGE_HORIZONTAL 20
#define DECALAGE_VERTICAL 40
#define LARGEUR_ECRAN (NOMBRE_COLONNES_GRILLE * TAILLE_CELLULE_GRILLE + 40)
#define HAUTEUR_ECRAN (NOMBRE_LIGNES_GRILLE * TAILLE_CELLULE_GRILLE + 200)
#define NOMBRE_PIONS_MAX 8
// definition des camps
#define CAMP_1 1
#define CAMP_2 2
// Couleurs des camps
#define CAMP_1_COULEUR RED
#define CAMP_2_COULEUR BLUE
#define COULEUR_LETTRE_PION WHITE
#define TAILLE_FONTE_LETTRE_PION 26
#define COULEUR_SELECTION_PION YELLOW

// Enumération pour les types de pièces
typedef enum {
    SOLDAT = 0,
    ARCHER = 1,
    FILOU = 2
} typePion;

// Tableau de noms correspondant aux types de pièces
extern const char *nomPieces[];

// Structure pour représenter un pion sur la grille
typedef struct {
    char nomCourt[2];       // Nom en une lettre du pion
    int positionLigne;      // position actuell de la ligne
    int positionColonne;    // position actuell de la ligne
    Vector2 position;       // Position actuelle du pion en veteur pour l'affichage
    typePion type;          // Type de pion
    Color couleur;          // Couleur du pion
    bool estSelectionne;    // Indique si le pion est sélectionné
    int camp;               // Camp auquel le pion appartient (1 ou 2)
    int pointsDeVieMax;     // Points de vie maximum du pion
    int attaque;            // Valeur d'attaque du pion
    int defense;            // Valeur de défense du pion
    int deplacement;        // Nombre de cases pour se déplacer
    int portee;             // Portée du pion
} pionGrille;

// Fonctions utlisees
void initialiserJeu();
void boucleJeu();
void finJeu();
void deplacerPion(pionGrille *pion, int ligneCible, int colonneCible) ;

// Variables externes exposees et utlisees ailleurs
extern pionGrille pions[NOMBRE_PIONS_MAX]; // Tableau de pions
extern Rectangle grille[NOMBRE_LIGNES_GRILLE][NOMBRE_COLONNES_GRILLE]; // Grille
extern pionGrille *pionSelectionne; // Pointeur vers le pion sélectionné
extern bool deplacementPossible; // Indique si le dernier déplacement est possible
extern int tourActuel; // Camp actuel qui joue (1 ou 2)

#endif
