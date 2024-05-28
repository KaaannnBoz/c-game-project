/**************************************************************************
 * Nom du fichier : game.c
 * Description    : Contient toutes les fonctions principales de gestion du jeu
 * 					Contien aussi toutes les variables du jeu utilises dans deplacements.c, initialisations.c, attaques.c
 * Auteurs        : Team GEGK
 * Historique     :
 *      1/05/2024 : Création initiale des fonctions
 *		23/5/2024 : Mise en place du fichier game.c seul avec retrait de toutes les autres fonctions 
 *		25/5/2024 : Ajout options IA et appel IA
 * Liste des fonctions :
 *		- boucleJeu() : boucle principale du jeu
 *		- afficherPopupFin affichadu poup de fin de jeu quand c'est terminé
 * 		- verifiePionsMorts() : permet de vérifier si tous les pions d'un camp sont morts (pour fin de jeu)
 * 		- calculerPointsDeVie() : calcul le total des pints de vioe dans les deux camps
 *		- estJeuFini() : indique pour la boucle principale si le jeu est fini
 * 		- finJeu() : fonction a appelé en fin de jeu pour libérer la memoire
 *		- affichePionsDebug() : Fonction de debug
 **************************************************************************/

#include "game.h"
#include "graphics.h"
#include "deplacements.h"
#include "attaques.h"
#include "player_ia.h"
#include <time.h>

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
int optionsIA = 1; // Options de l'IA vaut 1 ou 3 coups (au debut 1)
int typeJeu = JEU_DEUX_JOUEURS; // Indique si on fait un jeu un joueur, deux joueurs ou deux IA

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
    clock_t lastTime = clock();  // Initialisation du dernier temps mesuré
    while (!WindowShouldClose()) {
        if (!estJeuFini()){
            if (typeJeu == JEU_DEUX_IA) { // On doit ralendir le jeu en mode 2 IA car ca va tro vite
                // Voir actuce https://fr.wikiversity.org/wiki/Fonctions_de_base_en_langage_C/time.h#:~:text=Fonction%20time()%20modifier,0%20seconde%20sous%20UNIX%2C%20UTC.
                if ((clock() - lastTime) > (TEMPORISATION * CLOCKS_PER_SEC / 1000)) { // permet de pas aller trop vite en mode deux IA
                    deplacement();
                    attaque();
                    lastTime = clock();  // Réinitialiser le dernier temps mesuré
                }
            } else {
                deplacement();
                attaque();
            }
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

    Rectangle popup = { LargeurEcran / 2 - 200, hauteurEcran / 2 - 75, 400, 250 };
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);
        DrawRectangleRec(popup, WHITE);

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
        TraceLog(LOG_INFO,"DEBUG pion[%d] nom=%s, camp=%d, pv=%d,type=%s ",i,
                 pionsGrille[i].nomCourt,
                 pionsGrille[i].camp,
                 pionsGrille[i].pointsDeVie,
                 nomPions[pionsGrille[i].type]
                 );
    }
}
