#include "game.h"
#include <stdio.h>
#include <stdlib.h>

///// INITIALISATIONS/////
// Fonction pour initialiser la grille
void initialiserGrille() {
    // Allocation de la grille
    grille = (Rectangle**) calloc(nombreLignesGrille, sizeof(Rectangle*));
    for (int i = 0; i < nombreLignesGrille; i++) {
        grille[i] = (Rectangle*) calloc(nombreColonnesGrille, sizeof(Rectangle));
    }

    for (int ligne = 0; ligne < nombreLignesGrille; ligne++) {
        for (int colonne = 0; colonne < nombreColonnesGrille; colonne++) {
            grille[ligne][colonne].x = DECALAGE_HORIZONTAL + colonne * TAILLE_CELLULE_GRILLE;
            grille[ligne][colonne].y = DECALAGE_VERTICAL + ligne * TAILLE_CELLULE_GRILLE;
            grille[ligne][colonne].width = TAILLE_CELLULE_GRILLE;
            grille[ligne][colonne].height = TAILLE_CELLULE_GRILLE;
        }
    }
}

// Fonction pour initialiser un soldat
void initialiserSoldat(pionGrille *pion, int ligne, int colonne, int camp, char nomCourt[2], int noPion) {
    pion->numeroPion = noPion;
    pion->positionLigne = ligne;
    pion->positionColonne = colonne;
    pion->position.x = DECALAGE_HORIZONTAL + colonne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->position.y = DECALAGE_VERTICAL + ligne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->type = SOLDAT;
    pion->couleur = (camp == 1) ? CAMP_1_COULEUR : CAMP_2_COULEUR;
    pion->estSelectionne = false;
    pion->camp = camp;
    pion->pointsDeVieMax = 5;
    pion->pointsDeVie = pion->pointsDeVieMax;
    pion->attaque = 2;
    pion->defense = 3;
    pion->deplacement = 2;
    pion->portee = 1;
    pion->nomCourt[0] = nomCourt[0];
    pion->nomCourt[1] = '\0';
}

// Fonction pour initialiser un archer
void initialiserArcher(pionGrille *pion, int ligne, int colonne, int camp,char nomCourt[2], int noPion) {
    pion->numeroPion = noPion;
    pion->positionLigne = ligne;
    pion->positionColonne = colonne;
    pion->position.x = DECALAGE_HORIZONTAL + colonne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->position.y = DECALAGE_VERTICAL + ligne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->type = ARCHER;
    pion->couleur = (camp == 1) ? CAMP_1_COULEUR : CAMP_2_COULEUR;
    pion->estSelectionne = false;
    pion->camp = camp;
    pion->pointsDeVieMax = 3;
    pion->pointsDeVie = pion->pointsDeVieMax;
    pion->attaque = 3;
    pion->defense = 1;
    pion->deplacement = 2;
    pion->portee = 3;
    pion->nomCourt[0] = nomCourt[0];
    pion->nomCourt[1] = '\0';
}

// Fonction pour initialiser un filou
void initialiserFilou(pionGrille *pion, int ligne, int colonne, int camp, char nomCourt[2], int noPion) {
    pion->numeroPion = noPion;
    pion->positionLigne = ligne;
    pion->positionColonne = colonne;
    pion->position.x = DECALAGE_HORIZONTAL + colonne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->position.y = DECALAGE_VERTICAL + ligne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->type = FILOU;
    pion->couleur = (camp == 1) ? CAMP_1_COULEUR : CAMP_2_COULEUR;
    pion->estSelectionne = false;
    pion->camp = camp;
    pion->pointsDeVieMax = 5;
    pion->pointsDeVie = pion->pointsDeVieMax;
    pion->attaque = 4;
    pion->defense = 0;
    pion->deplacement = 4;
    pion->portee = 1;
    pion->nomCourt[0] = nomCourt[0];
    pion->nomCourt[1] = '\0';
}

// Fonction pour initialiser les pions
void initialiserPions(pionGrille pions[]) {
    // Initialisation des pions dans le camp 1
    initialiserSoldat(&pions[0], 1, 1, CAMP_1,"B",0);
    initialiserSoldat(&pions[1], 3, 1, CAMP_1,"D",1);
    initialiserArcher(&pions[2], 1, 0, CAMP_1,"A",2);
    initialiserFilou(&pions[3], 3, 0, CAMP_1,"C",3);

    // Initialisation des pions dans le camp 2
    initialiserSoldat(&pions[4], 1, nombreColonnesGrille-2, CAMP_2,"W",0);
    initialiserSoldat(&pions[5], 3, nombreColonnesGrille-2, CAMP_2,"Y",1);
    initialiserArcher(&pions[6], 1, nombreColonnesGrille-1, CAMP_2,"X",2);
    initialiserFilou(&pions[7], 3, nombreColonnesGrille-1, CAMP_2,"Z",3);
}

// Initialisation du jeu
void initialiserJeu(){
    // Initialisation de la fenêtre Raylib avec une taille variable
    LargeurEcran = nombreColonnesGrille * TAILLE_CELLULE_GRILLE + 40;
    hauteurEcran = nombreLignesGrille * TAILLE_CELLULE_GRILLE + 310;


    //InitWindow(LargeurEcran, hauteurEcran, "Partie à deux joeurs");
    // On remplace l'init de la fenetre par le changement de taille de la fentre
    SetWindowSize(LargeurEcran, hauteurEcran); // Changement de la taille de la fentre pour que cela comme la grille
    // Initialisation de la grille
    initialiserGrille();
    // Initialisation des pions
    initialiserPions(pionsGrille);

    pionSelectionne = NULL; // Par defaut pas de pion selectionne
    denierPionSelectionne = NULL; // Pas de dernier pion
    deplacementPossible = true; // Indique si le dernier deplacement est possible - on commance par deplacement
    tourActuel = 1; // Par défaut le premeir camp qui joue est le camp 1
    deplacementFait = false;
    nombreCoups= 0;
    toutLesPionsMortCamp1 = false;
    toutLesPionsMortCamp2 = false;
}
