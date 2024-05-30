/**************************************************************************
 * Nom du fichier : graphics.c
 * Description    : Contient les fonctions de rendu graphique du jeu
 * Auteurs        : Team GEGK
 * Historique     :
 *      1/05/2024 : Création initiale du fichier 
 *		23/05/2024 : Déplacement dans graphics.c de la boucle principale du jeu
 * Liste des fonctions :
 *		-renduGraphique : rendu graphique de la boucle principale du jeu
 **************************************************************************/
 
#include "graphics.h"

////////////// RENDU GRAPHIQUE /////////////

void renduGraphique(){
    // Début du rendu graphique
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Dessin de la grille
    for (int ligne = 0; ligne < nombreLignesGrille; ligne++) {
        for (int colonne = 0; colonne < nombreColonnesGrille; colonne++) {
            if ((ligne + colonne) % 2 == 0) { // On met des couleurs differentes dans les cases
                DrawRectangleRec(grille[ligne][colonne], DARKGRAY);
            } else {
                DrawRectangleRec(grille[ligne][colonne], LIGHTGRAY);
            }
            // On met un tour autour des cases
            DrawRectangleLinesEx(grille[ligne][colonne], 2, BLACK);
        }
    }
    // Dessin des pions
    // Le nombre de pions est aussi sizeof(pions) / sizeof(pions[0])
    for (int i = 0; i < NOMBRE_PIONS_MAX ; i++) {
        // Dessin du pion avec la bonne colueur
        if (pionsGrille[i].estFatigue){
            DrawCircleV(pionsGrille[i].position, TAILLE_CELLULE_GRILLE / 4, COULEUR_PION_FATIGUE);
        } else {
            DrawCircleV(pionsGrille[i].position, TAILLE_CELLULE_GRILLE / 4, pionsGrille[i].couleur);
        }

        // Le pion selectionne apparait de maniere diffrente
        if (pionsGrille[i].estSelectionne) {
            DrawCircleLines(pionsGrille[i].position.x, pionsGrille[i].position.y, TAILLE_CELLULE_GRILLE / 4 + 4, COULEUR_SELECTION_PION);
        }

        // Dessiner la première lettre du nom du type de pion au milieu du dessin du pion
        // Ppur pouvoir mettre au milieu on se sert de la fonction qui permet de donner la taille du texte
        // Astuce Reddit : https://www.reddit.com/r/raylib/comments/rh3zm4/center_text/
        DrawText( pionsGrille[i].nomCourt, pionsGrille[i].position.x - MeasureText( pionsGrille[i].nomCourt, TAILLE_FONTE_LETTRE_PION) / 2, pionsGrille[i].position.y - TAILLE_FONTE_LETTRE_PION/2, TAILLE_FONTE_LETTRE_PION, COULEUR_LETTRE_PION);
    }

    // Affichage dans la zone de texte
    // A a partir du bas
    DrawRectangle(0, hauteurEcran - 120, LargeurEcran, 120, LIGHTGRAY);
    DrawText(TextFormat("Camp actuel qui joue : %d (%s)", tourActuel,deplacementFait?"Attaque":"Deplacement"), 10, hauteurEcran - 105, 16, tourActuel==CAMP_1?CAMP_1_COULEUR:CAMP_2_COULEUR);
    if (pionSelectionne != NULL) {
        DrawText("Pion selectionné : ", 10, hauteurEcran - 85, 16, BLACK);
        DrawText(nomPions[pionSelectionne->type], 149, hauteurEcran - 85, 16, BLACK);
        DrawText(TextFormat("PV restants: %01i", pionSelectionne->pointsDeVie), 10, hauteurEcran - 65, 16, BLACK);
        DrawText(TextFormat("Attaque: %01i", pionSelectionne->attaque), 10, hauteurEcran - 45, 16, BLACK);
        DrawText(TextFormat("Defense: %01i", pionSelectionne->defense), 10, hauteurEcran - 25, 16, BLACK);
    } else {
        DrawText("Aucun pion selectionné", 10, hauteurEcran - 85, 16, BLACK);
    }

    // Message d'erreur pour texte interdit
    if (!deplacementPossible) {
        DrawText("Déplacement interdit", 200, hauteurEcran - 25, 16, RED);
    }

    // Fin du rendu graphique
    EndDrawing();
}