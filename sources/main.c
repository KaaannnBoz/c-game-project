/**************************************************************************
 * Nom du fichier : main.c
 * Description    : Fonction principal du kjeu pour son lancement
 * Auteurs        : Team GEGK
 * Historique     :
 *      1/5/2024 : Création initiale du fichier 
 *		5/5/2024 : Ajout jeu deux joueurs
 *		25/5/2024 : Ajout IA pour un joueur
 * Liste des fonctions :
 *		- main : Main du programme
 **************************************************************************/

#include <raylib.h>
#include "game.h"
#include "graphics.h"
#include "player_ia.h"
#include "menu.h"

int main() {
    // Initialisation de la fenêtre Raylib
    const int screenWidth = 800;
    const int screenHeight = 600;
    nombreLignesGrille = NOMBRE_LIGNES_GRILLE; // Nombre de lignes par defaut
    nombreColonnesGrille = NOMBRE_COLONNES_GRILLE; // Nombre de colonnes par defaut

    InitWindow(screenWidth, screenHeight, "GEGK : Un affrontement stratégique !");
    SetTargetFPS(FRAME_RATE);

    int option = JEU_AUCUN;
    while (1) { // Boucle infinie tant qu'on fait pas quitter
        // Affichage du menu et sélection de l'option
        switch (option) {
            case JEU_AUCUN :
                option = ShowMenu();
                break;
            case JEU_QUITTER :
                CloseWindow();
                TraceLog(LOG_INFO,"FIN MAIN");
                return 0; // Quitter le jeu
            case JEU_DEUX_JOUEURS :
                initialiserJeu(JEU_DEUX_JOUEURS); // Initialisation du jeu
                boucleJeu(); // Boucle principale du jeu
                finJeu(); // fin du jeu
                option = JEU_AUCUN;
                TraceLog(LOG_INFO,"FIN JEU_DEUX_JOUEURS");
                break;
            case JEU_UN_JOUEUR :
                initialiserJeu(JEU_UN_JOUEUR); // Initialisation du jeu
                boucleJeu(); // Boucle principale du jeu
                finJeu(); // fin du jeu
                option = JEU_AUCUN;
                TraceLog(LOG_INFO,"FIN JEU_UN_JOUEUR");			
                break;
            case JEU_A_PROPOS :
                break;
            case JEU_DEUX_IA :
                initialiserJeu(JEU_DEUX_IA); // Initialisation du jeu
                boucleJeu(); // Boucle principale du jeu
                finJeu(); // fin du jeu
                option = JEU_AUCUN;
                TraceLog(LOG_INFO,"FIN JEU_DEUX_IA");
                break;
        }
    }
    CloseWindow();
    TraceLog(LOG_INFO,"FIN MAIN");
    return 0;
}
