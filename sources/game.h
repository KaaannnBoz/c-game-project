/**************************************************************************
 * Nom du fichier : game.h
 * Description    : Contient toutes les fonctions principales de gestion du jeu
 * 					Contien aussi toutes les variables du jeu utilises dans deplacements.c, initialisations.c, attaques.c
 * 					Contient les types principaux utlisés dans le jeu : pionGrille et les constantes 
 * Auteurs        : Team GEGK
 **************************************************************************/

#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//#include <math.h>
//#include <ctype.h>
#include "menu.h"

#define NOMBRE_COLONNES_GRILLE 7 // Valeur par defaut du nombre de colonnes
#define NOMBRE_LIGNES_GRILLE 5 // Valeur par defaut du nombre de lignes
#define MAX_NOMBRE_COLONNES_GRILLE 22 // Valeur  max du nombre de colonnes
#define MIN_NOMBRE_COLONNES_GRILLE 4 // Valeur par defaut du nombre de lignes
#define MAX_NOMBRE_LIGNES_GRILLE 9 // Valeur  max du nombre de colonnes
#define MIN_NOMBRE_LIGNES_GRILLE 4 // Valeur par defaut du nombre de lignes
#define TAILLE_CELLULE_GRILLE 80
#define DECALAGE_HORIZONTAL 20
#define DECALAGE_VERTICAL 40
#define NOMBRE_PIONS_MAX 8
#define MAX_COUPS 100 // Nombre de coups maximum dans une partie
#define FRAME_RATE 60 // Pour Raylib taux de raffraichissssement
#define TEMPORISATION 500 // 500 ms pour la tempo en mode deux IA

// definition des camps
#define CAMP_1 1
#define CAMP_2 2
// Couleurs des camps
#define CAMP_1_COULEUR RED
#define CAMP_2_COULEUR BLUE
#define CAMP_1_COULEUR_PION_FATIGUE BROWN
#define CAMP_2_COULEUR_PION_FATIGUE DARKPURPLE
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
    int numeroPion;         // Numéro du pion
    char nomCourt[2];       // Nom en une lettre du pion
    int positionLigne;      // position actuell de la ligne
    int positionColonne;    // position actuell de la ligne
    Vector2 position;       // Position actuelle du pion en veteur pour l'affichage
    typePion type;          // Type de pion
    Color couleur;          // Couleur du pion
    Color couleurPionFatigue; // Couleur du pion Fatigue
    bool estSelectionne;    // Indique si le pion est sélectionné
    int camp;               // Camp auquel le pion appartient (1 ou 2)
    int pointsDeVieMax;     // Points de vie maximum du pion
    int pointsDeVie;        // Points de courant du pion
    int attaque;            // Valeur d'attaque du pion
    int defense;            // Valeur de défense du pion
    int deplacement;        // Nombre de cases pour se déplacer
    int portee;             // Portée du pion
    bool estFatigue;        // Indique si le pion est fatigué
} pionGrille;

// Fonctions utlisees
void initialiserJeu();
void boucleJeu();
void finJeu();
void deplacerPion(pionGrille *pion, int ligneCible, int colonneCible) ;
void affichePionsDebug(pionGrille pions[]);

// Variables externes exposees et utlisees ailleurs
extern int nombreLignesGrille; // Nombre de lignes de la grille
extern int nombreColonnesGrille; // Nombre de colonnes de la grille
extern pionGrille pionsGrille[NOMBRE_PIONS_MAX]; // Tableau de pions
extern Rectangle** grille; // Grille
extern pionGrille *pionSelectionne; // Pointeur vers le pion sélectionné
extern bool deplacementPossible; // Indique si le dernier déplacement est possible
extern int tourActuel; // Camp actuel qui joue (1 ou 2)

extern int LargeurEcran;
extern int hauteurEcran;
extern int nombreCoups; // Permet de savoir combien de coups ont été joues
extern bool toutLesPionsMortCamp1; //Permet de savoir si tout les Pions du Camp 1 sont Mort
extern bool toutLesPionsMortCamp2; //Permet de savoir si tout les Pions du Camp 2 sont Mort
extern int totalPvCamp1; // Permet de conaitre les PV du camp 1
extern int totalPvCamp2;// Permet de conaitre les PV du camp 2
extern bool deplacementFait; // Indique si le derner deplacment fait
extern pionGrille *denierPionSelectionne; // Pointeur vers le dernier pion sélectionné (le dernier deplace)
extern const char *nomPions[];
extern int optionsIA; // Options de l'IA vaut 1 ou 3 coups
extern int typeJeu ; // Indique si on fait un jeu un joueur, deux joueurs ou deux IA

#endif
