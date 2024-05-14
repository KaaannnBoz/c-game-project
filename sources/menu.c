#include "menu.h"
#include "raylib.h"

// Fonction permettant d'afficher le menu principal
// La fonction renvoie l'item de menu séléctionné

int ShowMenu(void) {
    int option = JEU_AUCUN; // Par défaut on a rien selectionné
    int currentMenu = JEU_AUCUN; 
    bool selectionMenu = false;    

    int largeurEcran = GetScreenWidth(); // récupération de la largeur de l'écran principale
    int hauteurEcran = GetScreenHeight(); // récupération de la hauteur de l'écran principale

    Image imOrigin = LoadImage("../assets/menu1.png");                   // Chargement de l'image en mémoire
    ImageFormat(&imOrigin, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);          // Passage de l'image en RGBA 32bit <-- Conseil dans exemples de Raylib
    Texture2D texture = LoadTextureFromImage(imOrigin);                 // Conversion de l'aimage en texture pour son affichage

    Rectangle itemMenuRec[NOMBRE_ITEMS_MENU] = { 0 }; // Initialisation des élements du menu
    int sourisSurItemMenu = -1; // La souris n'est sur aucun menu par défaut

    // Affichage des menus le nombre hauteur_menu est à ajuster pour la hauteur du menu
    const int hauteur_menu = 170;
    for (int i = 0; i < NOMBRE_ITEMS_MENU; i++) itemMenuRec[i] = (Rectangle){ 40.0f, (float)(hauteur_menu + 32*i), 150.0f, 30.0f };

    SetTargetFPS(60);

    while (!WindowShouldClose())    // Boucle tant que l'on a pas appuyé sur "Esc" ou la "X"
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

        // Permet de gérer les mouvements avec le clavier
        // Quand on arrive en haut ou en bas on fait un tour
        if (IsKeyPressed(KEY_DOWN))
        {
            currentMenu++;
            if (currentMenu > (NOMBRE_ITEMS_MENU - 1)) currentMenu = 0;
            selectionMenu = false;
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
                    UnloadTexture(texture);       // Déchargement des images
                    UnloadImage(imOrigin);        // Déchargement des images
                    return option;
                    break;                    
            }
        }

        //----------------------------------------------------------------------------------
        // DESSINE LES MENUS
        //----------------------------------------------------------------------------------
        BeginDrawing();
 
            ClearBackground(RAYWHITE);

            DrawText("BIENVENUE DANS GEGK !", 170, 30, 30, DARKGRAY);

            // Dessin des rectangles et ajout du texte
            for (int i = 0; i < NOMBRE_ITEMS_MENU; i++)
            {
                DrawRectangleRec(itemMenuRec[i], ((i == currentMenu) || (i == sourisSurItemMenu)) ? SKYBLUE : LIGHTGRAY);
                DrawRectangleLines((int)itemMenuRec[i].x, (int) itemMenuRec[i].y, (int) itemMenuRec[i].width, (int) itemMenuRec[i].height, ((i == currentMenu) || (i == sourisSurItemMenu)) ? BLUE : GRAY);
                DrawText( menuText[i], (int)( itemMenuRec[i].x + itemMenuRec[i].width/2 - MeasureText(menuText[i], 10)/2), (int) itemMenuRec[i].y + 11, 10, ((i == currentMenu) || (i == sourisSurItemMenu)) ? DARKBLUE : DARKGRAY);
            }

            DrawTexture(texture, largeurEcran - texture.width - 60, hauteurEcran/2 - texture.height/2, WHITE);
            DrawRectangleLines(largeurEcran - texture.width - 60, hauteurEcran/2 - texture.height/2, texture.width, texture.height, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    UnloadTexture(texture);       // Déchargement des images
    UnloadImage(imOrigin);        // Déchargement des images
}
