#include "game.h"
#include "graphics.h"
#include "deplacements.h"
#include "attaques.h"
#include "player_ia.h"

pionGrille pionsGrille[NOMBRE_PIONS_MAX]; // Tableau de pions
Rectangle** grille; // Grille dynamique
pionGrille *pionSelectionne; // Pointeur vers le pion sélectionné
pionGrille *denierPionSelectionne; // Pointeur vers le dernier pion sélectionné (le dernier deplace)
bool deplacementPossible; // Indique si le dernier déplacement est possible
bool deplacementFait; // Indique si le derner deplacment fait
int tourActuel; // Camp actuel qui joue (1 ou 2)
int nombreLignesGrille; // Nombre de lignes de la grille
int nombreColonnesGrille; // Nombre de colonnes de la grille
int LargeurEcran;
int hauteurEcran;
int nombreCoups; // Permet de savoir combien de coups ont été joues
bool toutLesPionsMortCamp1; //Permet de savoir si tout les Pions du Camp 1 sont Mort
bool toutLesPionsMortCamp2; //Permet de savoir si tout les Pions du Camp 2 sont Mort
int totalPvCamp1; // Permet de conaitre les PV du camp 1
int totalPvCamp2;// Permet de conaitre les PV du camp 2

void affichePionsDebug();
void afficherPopupFin();
bool estJeuFini();

// Tableau de noms correspondant aux types de pions
const char *nomPions[] = {
        "Soldat",
        "Archer",
        "Filou"
};


///// BOUCLE PRINCIPALE DU JEU /////
void boucleJeu() {
    while (!WindowShouldClose()) {
        if (!estJeuFini()){
            deplacement();
            attaque();
            renduGraphique();
        }
        else {
            afficherPopupFin();
            return;
        }
    }
}

void afficherPopupFin(){
    // Créer le message de fin de partie
    //InitWindow(300, 300, "GEGK : FIN !");
    Rectangle popup = { LargeurEcran / 2 - 200, hauteurEcran / 2 - 75, 400, 250 };
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);
        DrawRectangleRec(popup, WHITE);

        //DrawText("Press ESC to close", popup.x + 20, popup.y + 60, 20, RAYWHITE);

        if (toutLesPionsMortCamp1 || toutLesPionsMortCamp2) {
            if (toutLesPionsMortCamp1) {
                DrawText("Le camp 1 a gagné !", popup.x + 50, popup.y + 20, 30, CAMP_1_COULEUR);
            } else {
                DrawText("Le camp 2 a gagné !", popup.x + 50, popup.y + 20, 30, CAMP_2_COULEUR);
            }
        }
        else {
            // Camp a égalité
            if(totalPvCamp1 > totalPvCamp2){
                DrawText("Le camp 1 a gagné !", popup.x + 50, popup.y + 20, 30, CAMP_1_COULEUR);
                DrawText("Par PV restant !", popup.x + 80, popup.y + 85, 30, CAMP_1_COULEUR);
                DrawText(TextFormat("PV camp 1 : %d PV camp 2 : %d",totalPvCamp1,totalPvCamp2), popup.x + 85, popup.y + 130, 15, CAMP_1_COULEUR);
            }
            else if(totalPvCamp1 < totalPvCamp2){
                    DrawText("Le camp 2 a gagné !", popup.x + 50, popup.y + 20, 30, CAMP_2_COULEUR);
                    DrawText("Par PV restant !", popup.x + 80, popup.y + 85, 30, CAMP_2_COULEUR);
                DrawText(TextFormat("PV camp 1 : %d PV camp 2 : %d",totalPvCamp1,totalPvCamp2), popup.x + 85, popup.y + 130, 15, CAMP_2_COULEUR);
                } else{
                DrawText("Le camp 2 a gagné !", popup.x + 50, popup.y + 20, 30, CAMP_2_COULEUR);
                DrawText("Par egalite !", popup.x + 85, popup.y + 80, 30, CAMP_2_COULEUR);
                DrawText(TextFormat("PV camp 1 : %d PV camp 2 : %d",totalPvCamp1,totalPvCamp2), popup.x + 85, popup.y + 130, 15, CAMP_2_COULEUR);
            }
        };
        DrawText("Appuyez sur la croix pour sortir", popup.x + 30, popup.y + 180, 20, BLACK);
        EndDrawing();
        // Attendre que l'utilisateur ferme la fenêtre
    }
    //CloseWindow();
}


// Fin du jeu
void verifiePionsMorts() {
    // Vérifier si tous les pions du camp 1 sont morts
    bool tousMortsCamp1 = true;
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
        if (pionsGrille[i].camp == CAMP_1 && pionsGrille[i].pointsDeVie > 0) {
            tousMortsCamp1 = false;
            break;
        }
    }
    if (tousMortsCamp1) {
        toutLesPionsMortCamp1 = true;
    }

    // Vérifier si tous les pions du camp 2 sont morts
    bool tousMortsCamp2 = true;
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
        if (pionsGrille[i].camp == CAMP_2 && pionsGrille[i].pointsDeVie > 0) {
            tousMortsCamp2 = false;
            break;
        }
    }
    if (tousMortsCamp2) {
        toutLesPionsMortCamp2 = true;
    }
}
void calculerPointsDeVie(){
    int PVCamp1 = 0;
    int PVCamp2 = 0;
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
        if (pionsGrille[i].camp == CAMP_2) {
            if (pionsGrille[i].pointsDeVie > 0){
                PVCamp2+= pionsGrille[i].pointsDeVie;
            }
        }
        else {
            if (pionsGrille[i].pointsDeVie > 0){
                PVCamp1+= pionsGrille[i].pointsDeVie;
            }
        }
    }

    totalPvCamp1 = PVCamp1;
    totalPvCamp2 = PVCamp2;

}
// si on dépase le nombre de coups on s'arrette
bool estJeuFini(){
    if (nombreCoups >= MAX_COUPS) {
        calculerPointsDeVie();
        return true;
    }
    verifiePionsMorts();
    if (toutLesPionsMortCamp1 || toutLesPionsMortCamp2) {
        TraceLog(LOG_INFO, "[verifiePionsMorts] toutLesPionsMortCamp2=%d,toutLesPionsMortCamp1=%d",toutLesPionsMortCamp2,toutLesPionsMortCamp1);
        return true;
    }
    return false;
}

void finJeu() {
    // Libération de la mémoire
    TraceLog(LOG_INFO,"==>finJeu free ==> nb lignes %d",nombreLignesGrille);
    for (int i = 0; i < nombreLignesGrille; i++) {
        free(grille[i]);
    }
    free(grille);
    TraceLog(LOG_INFO,"<==finJeu");
}

///////// DEBUG ////////////

// Fonction de debug - affichage etat des pions
void affichePionsDebug(pionGrille pions[]){
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
        TraceLog(LOG_INFO, "==> affichePionsDebug");
        TraceLog(LOG_INFO,"pion[%d].positionColonne = %d",i,pionsGrille[i].positionColonne);
        TraceLog(LOG_INFO,"pion[%d].positionLigne = %d",i,pions[i].positionLigne);
        TraceLog(LOG_INFO, "<== affichePionsDebug");
    }
}
