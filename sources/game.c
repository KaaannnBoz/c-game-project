#include "game.h"
#include "player_ia.h"
#include <stdio.h>
#include <stdlib.h>

pionGrille pions[NOMBRE_PIONS_MAX]; // Tableau de pions
Rectangle** grille; // Grille dynamique
pionGrille *pionSelectionne; // Pointeur vers le pion sélectionné
bool deplacementPossible; // Indique si le dernier déplacement est possible
int tourActuel; // Camp actuel qui joue (1 ou 2)
int nombreLignesGrille; // Nombre de lignes de la grille
int nombreColonnesGrille; // Nombre de colonnes de la grille
int LargeurEcran;
int hauteurEcran;

void affichePionsDebug();
// Tableau de noms correspondant aux types de pions
const char *nomPions[] = {
    "Soldat",
    "Archer",
    "Filou"
};

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
void initialiserSoldat(pionGrille *pion, int ligne, int colonne, int camp, char nomCourt[2]) {
    pion->positionLigne = ligne;
    pion->positionColonne = colonne;
    pion->position.x = DECALAGE_HORIZONTAL + colonne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->position.y = DECALAGE_VERTICAL + ligne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->type = SOLDAT;
    pion->couleur = (camp == 1) ? CAMP_1_COULEUR : CAMP_2_COULEUR;
    pion->estSelectionne = false;
    pion->camp = camp;
    pion->pointsDeVieMax = 5;
    pion->attaque = 2;
    pion->defense = 3;
    pion->deplacement = 2;
    pion->portee = 1;
    pion->nomCourt[0] = nomCourt[0];
    pion->nomCourt[1] = '\0';
}

// Fonction pour initialiser un archer
void initialiserArcher(pionGrille *pion, int ligne, int colonne, int camp,char nomCourt[2]) {
    pion->positionLigne = ligne;
    pion->positionColonne = colonne;
    pion->position.x = DECALAGE_HORIZONTAL + colonne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->position.y = DECALAGE_VERTICAL + ligne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->type = ARCHER;
    pion->couleur = (camp == 1) ? CAMP_1_COULEUR : CAMP_2_COULEUR;
    pion->estSelectionne = false;
    pion->camp = camp;
    pion->pointsDeVieMax = 3;
    pion->attaque = 3;
    pion->defense = 1;
    pion->deplacement = 2;
    pion->portee = 3;
    pion->nomCourt[0] = nomCourt[0];
    pion->nomCourt[1] = '\0';  
}

// Fonction pour initialiser un filou
void initialiserFilou(pionGrille *pion, int ligne, int colonne, int camp, char nomCourt[2]) {
    pion->positionLigne = ligne;
    pion->positionColonne = colonne;
    pion->position.x = DECALAGE_HORIZONTAL + colonne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->position.y = DECALAGE_VERTICAL + ligne * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    pion->type = FILOU;
    pion->couleur = (camp == 1) ? CAMP_1_COULEUR : CAMP_2_COULEUR;
    pion->estSelectionne = false;
    pion->camp = camp;
    pion->pointsDeVieMax = 5;
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
    initialiserSoldat(&pions[0], 1, 1, CAMP_1,"B");
    initialiserSoldat(&pions[1], 3, 1, CAMP_1,"D");
    initialiserArcher(&pions[2], 1, 0, CAMP_1,"A");
    initialiserFilou(&pions[3], 3, 0, CAMP_1,"C");

    // Initialisation des pions dans le camp 2
    initialiserSoldat(&pions[4], 1, nombreColonnesGrille-2, CAMP_2,"W");
    initialiserSoldat(&pions[5], 3, nombreColonnesGrille-2, CAMP_2,"Y");
    initialiserArcher(&pions[6], 1, nombreColonnesGrille-1, CAMP_2,"X");
    initialiserFilou(&pions[7], 3, nombreColonnesGrille-1, CAMP_2,"Z");
}

// Initialisation du jeu
void initialiserJeu(){
    // Initialisation de la fenêtre Raylib avec une taille variable
    LargeurEcran = nombreColonnesGrille * TAILLE_CELLULE_GRILLE + 40;
    hauteurEcran = nombreLignesGrille * TAILLE_CELLULE_GRILLE + 200;


    //InitWindow(LargeurEcran, hauteurEcran, "Partie à deux joeurs");
    // On remplace l'init de la fenetre par le changement de taille de la fentre
    SetWindowSize(LargeurEcran, hauteurEcran); // Changement de la taille de la fentre pour que cela comme la grille
    // Initialisation de la grille
    initialiserGrille();
    // Initialisation des pions
    initialiserPions(pions);

    pionSelectionne = NULL; // Par defaut pas de pion selectionne
    deplacementPossible = true; // Indique si le dernier dé=eplacement est possible
    tourActuel = 1; // Par défaut le premeir camp qui joue est le camp 1
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

// Algorithmes permettant de calcluler la distance entre deux cases
// Pour la distance Mannathan voir Wikipedia ==> https://fr.wikipedia.org/wiki/Distance_de_Manhattan
// Il existe aussi la distance Tchebychev ==> de https://fr.wikipedia.org/wiki/Distance_de_Tchebychev
bool estDeplacementPossibleLigneColonne(int deplacementMax,int ligneCourante, int colonneCourante,int ligneCible, int colonneCible){
    bool resultat = false;
    int deplacementX, deplacementY,distance;
    int algo = 2; // Pour l'instant on utilise Manhattan
    switch (algo){
        case 1 : // Classique
            deplacementX = abs(ligneCourante - ligneCible);
            deplacementY = abs(colonneCourante - colonneCible);
            TraceLog(LOG_INFO, "[distanceEntreCases] max=%d, dx=%d, dy=%d",deplacementMax,deplacementX,deplacementX);
            resultat = !(deplacementX > deplacementMax || deplacementY > deplacementMax);
        break;
        case 2 : // Alorithme de Manhattan
            distance = abs(ligneCible - ligneCourante) + abs(colonneCible - colonneCourante);
            TraceLog(LOG_INFO, "[distanceEntreCases] max=%d, d=%d",deplacementMax,distance);
            resultat = distance <= deplacementMax;
        break;
        case 3 : // Alorithme de Tchebychev
            deplacementX = abs(ligneCourante - ligneCible);
            deplacementY = abs(colonneCourante - colonneCible);
            distance = deplacementX>deplacementY?deplacementX:deplacementY; // Il s'agit du MAX des 2
            TraceLog(LOG_INFO, "[distanceEntreCases] max=%d, dx=%d, dy=%d",deplacementMax,deplacementX,deplacementX);
            resultat = distance <= deplacementMax;
            break;
    }
    TraceLog(LOG_INFO, "[distanceEntreCases] Possible = %s",(resultat?"OUI":"NON"));
    return resultat;
}

// Fonction pour vérifier si un déplacement est possible pour un pion donné
bool estDeplacementPossible(const pionGrille *pion, int ligneCible, int colonneCible) {
    // Vérifier si on est dans la grille
    if (ligneCible < 0 || ligneCible >= nombreLignesGrille || colonneCible < 0 || colonneCible >= nombreColonnesGrille) {
        return false; // Déplacement impossible (hors de la grille)
    }

    // Vérifier si le déplacement est possible
    if (!estDeplacementPossibleLigneColonne(pion->deplacement,pion->positionLigne,pion->positionColonne,ligneCible,colonneCible)) {
        return false; // Déplacement impossible
    }
    // Vérifier si la case de destination est occupee par un autre pion
    // On ne peut pas bouger les pions dun meme camp l'un sur l'autre
    // Si on veut autoriser les pions du même camp ==> pions[i].camp != pion->camp
    //affichePionsDebug();
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
        if (pions[i].positionColonne == colonneCible && pions[i].positionLigne == ligneCible) {
            return false; // Ligne occupee par un pion
        }
    }
    return true; // Déplacement possible
}


// Fonction pour déplacer un pion sur la grille si le déplacement est possible
void deplacerPion(pionGrille *pion, int ligneCible, int colonneCible) {
    // On met le pion au milieu du rectangle
    Vector2 centreCible;
    centreCible.x = DECALAGE_HORIZONTAL + colonneCible * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    centreCible.y = DECALAGE_VERTICAL + ligneCible * TAILLE_CELLULE_GRILLE + TAILLE_CELLULE_GRILLE / 2;
    // Déplacer le pion
    pion->position = centreCible;
    pion->positionLigne = ligneCible;
    pion->positionColonne = colonneCible;
}

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
        DrawCircleV(pions[i].position, TAILLE_CELLULE_GRILLE / 4, pions[i].couleur);
        
        // Le pion selectionne apparait de maniere diffrente
        if (pions[i].estSelectionne) {
            DrawCircleLines(pions[i].position.x, pions[i].position.y, TAILLE_CELLULE_GRILLE / 4 + 4, COULEUR_SELECTION_PION);
        }

        // Dessiner la première lettre du nom du type de pion au milieu du dessin du pion
        // Ppur pouvoir mettre au milieu on se sert de la fonction qui permet de donner la taille du texte
        // Astuce Reddit : https://www.reddit.com/r/raylib/comments/rh3zm4/center_text/
        DrawText( pions[i].nomCourt, pions[i].position.x - MeasureText( pions[i].nomCourt, TAILLE_FONTE_LETTRE_PION) / 2, pions[i].position.y - TAILLE_FONTE_LETTRE_PION/2, TAILLE_FONTE_LETTRE_PION, COULEUR_LETTRE_PION);
    }

    // Affichage dans la zone de texte
    // A a partir du bas
    DrawRectangle(0, hauteurEcran - 120, LargeurEcran, 120, LIGHTGRAY);
    DrawText(TextFormat("Camp actuel : %d", tourActuel), 10, hauteurEcran - 105, 16, BLACK);
    if (pionSelectionne != NULL) {
        DrawText("Pion selectionné : ", 10, hauteurEcran - 85, 16, BLACK);
        DrawText(nomPions[pionSelectionne->type], 180, hauteurEcran - 85, 16, BLACK);
    } else {
        DrawText("Aucun pion selectionné", 10, hauteurEcran - 85, 16, BLACK);
    }

    // Message d'erreur pour texte interdit
    if (!deplacementPossible) {
        DrawText("Déplacement interdit", 10, hauteurEcran - 60, 16, RED);
    }

    // Fin du rendu graphique
    EndDrawing();
}
void boucleJeu() {
    while (!WindowShouldClose()) {
        // Détection du clic de souris
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 positionSouris = GetMousePosition();

            // Vérifier si un pion est clique et le sélectionner
            for (int i = 0; i < sizeof(pions) / sizeof(pions[0]); i++) {
                if (CheckCollisionPointCircle(positionSouris, pions[i].position, TAILLE_CELLULE_GRILLE / 4) && pions[i].camp == tourActuel) {
                    // Désélectionner le pion  sélectionné avant
                    if (pionSelectionne != NULL) {
                        pionSelectionne->estSelectionne = false;
                    }

                    // Sélectionner le nouveau pion
                    pions[i].estSelectionne = true;
                    pionSelectionne = &pions[i]; // indique le nouveau pion selectionne
                    break;
                }
            }
        }

        // Déplacer le pion sélectionné avec le mouvement de la souris
        if (pionSelectionne != NULL && IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            // Obtenir les indices de ligne et de colonne de la case cible
            int ligneCible = (GetMouseY() - DECALAGE_VERTICAL) / TAILLE_CELLULE_GRILLE;
            int colonneCible = (GetMouseX() - DECALAGE_HORIZONTAL) / TAILLE_CELLULE_GRILLE;
            TraceLog(LOG_INFO,"CALCUL ligneCible=%d",ligneCible);
            TraceLog(LOG_INFO,"CALCUL colonneCible=%d",colonneCible);
            // Vérifier si le déplacement est valide
            deplacementPossible = estDeplacementPossible(pionSelectionne, ligneCible, colonneCible);

            if (deplacementPossible) {
                // Déplacer le pion sélectionné
                deplacerPion(pionSelectionne, ligneCible, colonneCible);

                // Réinitialiser la sélection après le déplacement
                pionSelectionne->estSelectionne = false;
                pionSelectionne = NULL;

                // Changer de camp pour le prochain tour
                tourActuel = (tourActuel == 1) ? 2 : 1;
            }
        }
        renduGraphique();
    }    
}
void affichePionsDebug(){
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
        TraceLog(LOG_INFO, "==> affichePionsDebug");
        TraceLog(LOG_INFO,"pion[%d].positionColonne = %d",i,pions[i].positionColonne);
        TraceLog(LOG_INFO,"pion[%d].positionLigne = %d",i,pions[i].positionLigne);
        TraceLog(LOG_INFO, "<== affichePionsDebug");
    }
}