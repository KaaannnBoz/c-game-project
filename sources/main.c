#include <raylib.h>
#include "game.h"
#include "graphics.h"
#include "player_ia.h"
#include "menu.h"

int main() {
    // Initialisation de la fenêtre Raylib
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "GEGK : Un affrontement stratégique !");

    // Affichage du menu et sélection de l'option
    int option = ShowMenu();
    switch(option)
    {
        case JEU_QUITTER :
            CloseWindow();
            return 0; // Quitter le jeu
        case JEU_DEUX_JOUEURS :    
            // Initialisation du jeu
            InitGame();
            // Boucle principale du jeu
            while (!WindowShouldClose()) {
                UpdateGame();
                DrawGame();
            }
            // Nettoyage à la fin du jeu
            EndGame();
            CloseWindow();
            return 0;
        case JEU_UN_JOUEUR :
            break;
         case JEU_DEUX_IA :
            break;
    }
}
