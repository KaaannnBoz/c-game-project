#include "menu.h"
#include "game.h"
#include "raylib.h"

// Fonction permettant d'afficher le menu principal
// La fonction renvoie l'item de menu séléctionné

int ShowMenu(void) {
    bool fermerFenetre = false ;
    int option = JEU_AUCUN; // Par défaut on a rien selectionné
    int currentMenu = JEU_AUCUN; 
    bool selectionMenu = false;
    const int screenWidth = 800;
    const int screenHeight = 600;
    SetWindowSize(screenWidth, screenHeight);
    int largeurEcran = GetScreenWidth(); // récupération de la largeur de l'écran principale
    int hauteurEcran = GetScreenHeight(); // récupération de la hauteur de l'écran principale

    // Images
    Image imOrigin = LoadImage("../assets/menu1.png");                   // Chargement de l'image en mémoire
    ImageFormat(&imOrigin, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);          // Passage de l'image en RGBA 32bit <-- Conseil dans exemples de Raylib
    Texture2D textureMenu = LoadTextureFromImage(imOrigin); // Passage de l'image en RGBA 32bit

    Image imgAPropos = LoadImage("../assets/apropos.png"); // chargement de l'image À PROPOS en mémoire
    ImageFormat(&imgAPropos, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8); // Formatage de l'image À PROPOS
    Texture2D textureAPropos = LoadTextureFromImage(imgAPropos); // Passage de l'image en RGBA 32bit

    // Options
    int optionsX = largeurEcran / 2+100;
    int optionsY = hauteurEcran / 2 -200;
    // Largeur grille
    Rectangle largeurGrilleBoutonPlus = { optionsX, optionsY, 30, 50 };
    Rectangle largeurGrilleBoutonMoins = { optionsX+35, optionsY, 30, 50 };
    // hauteur grille
    Rectangle hauteurGrilleBoutonPlus = { optionsX, optionsY+60, 30, 50 };
    Rectangle hauteurGrilleBoutonMoins = { optionsX+35, optionsY+60, 30, 50 };
    // Choix de l'IA
    Rectangle IABoutonPlus = { optionsX, optionsY+120, 30, 50 };
    Rectangle IABoutonMoins = { optionsX+35, optionsY+120, 30, 50 };
    int optionsIA = 3;

    // MENU
    Rectangle itemMenuRec[NOMBRE_ITEMS_MENU] = { 0 }; // Initialisation des élements du menu
    int sourisSurItemMenu = -1; // La souris n'est sur aucun menu par défaut

    // Affichage des menus le nombre hauteur_menu est à ajuster pour la hauteur du menu
    const int hauteur_menu = 170;
    for (int i = 0; i < NOMBRE_ITEMS_MENU; i++) itemMenuRec[i] = (Rectangle){ 40.0f, (float)(hauteur_menu + 32*i), 150.0f, 30.0f };

    SetTargetFPS(60);


    while (!fermerFenetre)    // Boucle tant que l'on a pas appuyé sur "Esc" ou la "X"
    {
        // Permet de determiner ce qui a été selection comme menu avec la souris
        for (int i = 0; i < NOMBRE_ITEMS_MENU; i++)
        {
            if (CheckCollisionPointRec(GetMousePosition(), itemMenuRec[i]))
            {
                sourisSurItemMenu = i;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    currentMenu = i;
                    selectionMenu = true;
                }
                break;
            }
            else sourisSurItemMenu = -1;
        }
        // Pour les options
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePoint = GetMousePosition();
            // largeur de la grille
            if (CheckCollisionPointRec(mousePoint, largeurGrilleBoutonPlus)) {
                nombreColonnesGrille = nombreColonnesGrille>=MAX_NOMBRE_COLONNES_GRILLE?MIN_NOMBRE_COLONNES_GRILLE:nombreColonnesGrille+1;
            }
            else if (CheckCollisionPointRec(mousePoint, largeurGrilleBoutonMoins)) {
                nombreColonnesGrille = nombreColonnesGrille<=MIN_NOMBRE_COLONNES_GRILLE?MIN_NOMBRE_COLONNES_GRILLE:nombreColonnesGrille-1;
            } else if (CheckCollisionPointRec(mousePoint, hauteurGrilleBoutonPlus)) { // Hauteur de la grille
                nombreLignesGrille = nombreLignesGrille>=MAX_NOMBRE_LIGNES_GRILLE?MIN_NOMBRE_LIGNES_GRILLE:nombreLignesGrille+1;
            }
            else if (CheckCollisionPointRec(mousePoint, hauteurGrilleBoutonMoins))
            {
                nombreLignesGrille = nombreLignesGrille<=MIN_NOMBRE_LIGNES_GRILLE?MIN_NOMBRE_LIGNES_GRILLE:nombreLignesGrille-1;
            } else if (CheckCollisionPointRec(mousePoint, IABoutonPlus)) { // Options de IA
                optionsIA = optionsIA>=3?1:optionsIA+1; // Quand on arrive a 3 on revien a 1
            }
            else if (CheckCollisionPointRec(mousePoint, IABoutonMoins))
            {
                optionsIA = optionsIA<=1?3:optionsIA-1; // Quand on arrive a 1 on revien a 3
            }
        }

        // Permet de gérer les mouvements avec le clavier
        // Quand on arrive en haut ou en bas on fait un tour
        if (IsKeyPressed(KEY_DOWN))
        {
            currentMenu++;
            if (currentMenu > (NOMBRE_ITEMS_MENU - 1)) currentMenu = 0;
            selectionMenu = false;
            // ... logique existante
            if (currentMenu != JEU_A_PROPOS && currentMenu != JEU_OPTIONS)  {
            }
        }
        else if (IsKeyPressed(KEY_UP))
        {
            currentMenu--;
            if (currentMenu < 0) currentMenu = NOMBRE_ITEMS_MENU;
            selectionMenu = false;
        }

        // Si on a bougé avec le clavier, il faut faitre "enter" pour seléctionner l'item du menu
        if (IsKeyPressed(KEY_ENTER)) {
            selectionMenu = true;
        }

        // Un menu a été sélectionné, il faut faire une action
        if (selectionMenu)
        {
            switch (currentMenu) {
                case JEU_QUITTER :
                case JEU_DEUX_JOUEURS :
                    option = currentMenu;
                    UnloadTexture(textureMenu);       // Déchargement des images
                    UnloadTexture(textureAPropos);
                    UnloadImage(imOrigin);// Déchargement des images
                    UnloadImage(imgAPropos);// Déchargement des images
                    fermerFenetre = true;
                    return option;
                    break;
                case JEU_OPTIONS:
                    option = currentMenu;
                    break;
                case JEU_A_PROPOS:
                    // Ne pas faire return option
                    option = currentMenu;
                    break;
            }
        }
        //----------------------------------------------------------------------------------
        // DESSIN
        //----------------------------------------------------------------------------------
        BeginDrawing();
 
            ClearBackground(RAYWHITE);

            // Dessin des rectangles et ajout du texte
            for (int i = 0; i < NOMBRE_ITEMS_MENU; i++)
            {
                DrawRectangleRec(itemMenuRec[i], ((i == currentMenu) || (i == sourisSurItemMenu)) ? SKYBLUE : LIGHTGRAY);
                DrawRectangleLines((int)itemMenuRec[i].x, (int) itemMenuRec[i].y, (int) itemMenuRec[i].width, (int) itemMenuRec[i].height, ((i == currentMenu) || (i == sourisSurItemMenu)) ? BLUE : GRAY);
                DrawText( menuText[i], (int)( itemMenuRec[i].x + itemMenuRec[i].width/2 - MeasureText(menuText[i], 10)/2), (int) itemMenuRec[i].y + 11, 10, ((i == currentMenu) || (i == sourisSurItemMenu)) ? DARKBLUE : DARKGRAY);
            }

            // Image du menu par defaut
            if (currentMenu != JEU_A_PROPOS && currentMenu != JEU_OPTIONS) {
                DrawText("BIENVENUE DANS GEGK !", 170, 30, 30, DARKGRAY);
                DrawTexture(textureMenu, largeurEcran - textureMenu.width - 60, hauteurEcran / 2 - textureMenu.height / 2, WHITE);
            }
            // Image du menu a propos
            if (currentMenu == JEU_A_PROPOS) {
                DrawText("A PROPOS", 170+200, 30, 30, DARKGRAY);
                DrawTexture(textureAPropos, largeurEcran - textureAPropos.width - 60, hauteurEcran / 2 - textureAPropos.height / 2, WHITE);
            }
            // Fonctions options
            if (currentMenu == JEU_OPTIONS) {
                DrawText("OPTIONS", 170+200, 30, 30, DARKGRAY);

                // Largeur de la grille
                DrawText(TextFormat("Largeur grille : %i", nombreColonnesGrille), largeurGrilleBoutonMoins.x - 250, largeurGrilleBoutonMoins.y + 15, 20, BLACK);
                DrawRectangleRec(largeurGrilleBoutonPlus, LIGHTGRAY);
                DrawText("+", largeurGrilleBoutonPlus.x + 10, largeurGrilleBoutonPlus.y + 15, 20, BLACK);
                DrawRectangleRec(largeurGrilleBoutonMoins, LIGHTGRAY);
                DrawText("-", largeurGrilleBoutonMoins.x + 10, largeurGrilleBoutonMoins.y + 15, 20, BLACK);

                // Hauteur de la grille
                DrawText(TextFormat("Hauteur grille : %i", nombreLignesGrille), hauteurGrilleBoutonMoins.x - 250, hauteurGrilleBoutonMoins.y + 15, 20, BLACK);
                DrawRectangleRec(hauteurGrilleBoutonPlus, LIGHTGRAY);
                DrawText("+", hauteurGrilleBoutonPlus.x + 10, hauteurGrilleBoutonPlus.y + 15, 20, BLACK);
                DrawRectangleRec(hauteurGrilleBoutonMoins, LIGHTGRAY);
                DrawText("-", hauteurGrilleBoutonMoins.x + 10, hauteurGrilleBoutonMoins.y + 15, 20, BLACK);

                // Choix de l'IA
                DrawText(TextFormat("Prévision IA : %i", optionsIA), IABoutonMoins.x - 250, IABoutonMoins.y + 15, 20, BLACK);
                DrawRectangleRec(IABoutonPlus, LIGHTGRAY);
                DrawText("+", IABoutonPlus.x + 10, IABoutonPlus.y + 15, 20, BLACK);
                DrawRectangleRec(IABoutonMoins, LIGHTGRAY);
                DrawText("-", IABoutonMoins.x + 10, IABoutonMoins.y + 15, 20, BLACK);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    UnloadTexture(textureAPropos);
    UnloadTexture(textureMenu);   // Déchargement des images
    UnloadImage(imOrigin);        // Déchargement des images
    UnloadImage(imgAPropos);// Déchargement des images
    TraceLog(LOG_INFO,"==>FIN MENU");
}
