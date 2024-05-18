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
                initialiserJeu(); // Initialisation du jeu
                boucleJeu(); // Boucle principale du jeu
                finJeu(); // fin du jeu
                option = JEU_AUCUN;
                TraceLog(LOG_INFO,"FIN JEU_DEUX_JOUEURS");
                break;
            case JEU_UN_JOUEUR :
                break;
            case JEU_A_PROPOS :
                break;
            case JEU_DEUX_IA :
                break;
        }
    }
    CloseWindow();
    TraceLog(LOG_INFO,"FIN MAIN");
    return 0;
}
