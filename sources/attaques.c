#include "game.h"

///////// ATTAQUES /////////////

// Algorithmes permettant de calcluler la distance entre deux cases
// Pour la distance Mannathan voir Wikipedia ==> https://fr.wikipedia.org/wiki/Distance_de_Manhattan
// Il existe aussi la distance Tchebychev ==> de https://fr.wikipedia.org/wiki/Distance_de_Tchebychev
bool estAttaquePossibleLigneColonne(int porteeMax,int ligneCourante, int colonneCourante,int ligneCible, int colonneCible){
    bool resultat = false;
    int deplacementX, deplacementY,distance;
    int algo = 1; // Pourla portée classique
    switch (algo){
        case 1 : // Classique
            deplacementX = abs(ligneCourante - ligneCible);
            deplacementY = abs(colonneCourante - colonneCible);
            TraceLog(LOG_INFO, "[estAttaquePossibleLigneColonne] max=%d, dx=%d, dy=%d",porteeMax,deplacementX,deplacementY);
            resultat = !(deplacementX > porteeMax || deplacementY > porteeMax);
            break;
        case 2 : // Alorithme de Manhattan
            distance = abs(ligneCible - ligneCourante) + abs(colonneCible - colonneCourante);
            TraceLog(LOG_INFO, "[estAttaquePossibleLigneColonne] max=%d, d=%d",porteeMax,distance);
            resultat = distance <= porteeMax;
            break;
        case 3 : // Alorithme de Tchebychev
            deplacementX = abs(ligneCourante - ligneCible);
            deplacementY = abs(colonneCourante - colonneCible);
            distance = deplacementX>deplacementY?deplacementX:deplacementY; // Il s'agit du MAX des 2
            TraceLog(LOG_INFO, "[estAttaquePossibleLigneColonne] max=%d, dx=%d, dy=%d",porteeMax,deplacementX,deplacementX);
            resultat = distance <= porteeMax;
            break;
    }
    TraceLog(LOG_INFO, "[estAttaquePossibleLigneColonne] Possible = %s",(resultat?"OUI":"NON"));
    return resultat;
}
// Fonction pour vérifier si une attaque est possible pour un pion donné
// Il renvoie le pion attaqué
pionGrille* estAttaquePossible(int ligneCible, int colonneCible,pionGrille pions[]) {
    // Vérifier si la case de destination est occupee par un autre pion
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
        if (pions[i].positionColonne == colonneCible && pions[i].positionLigne == ligneCible) {
            if (pions[i].camp != tourActuel) { // On peut attaque un pion que si dans l'autre camp
                if (pions[i].pointsDeVie > 0) { // On attaque pas les morts
                    if (estAttaquePossibleLigneColonne(denierPionSelectionne->portee,
                                                       denierPionSelectionne->positionLigne,
                                                       denierPionSelectionne->positionColonne, ligneCible,
                                                       colonneCible)) {
                        return &pions[i]; // Ligne non occupee par un pion et a portée donc on attaque
                    }
                }
            }
        }
    }
    return NULL; // Attaque impossible
}

// Attaquer du pion
void attaquerPion(pionGrille* pionQuiAttaque,pionGrille* pionAttaque){
    TraceLog(LOG_INFO,"[attaquerPion] DEBUT");
    TraceLog(LOG_INFO,"[attaquerPion] pion %s attaque %s",pionQuiAttaque->nomCourt,pionAttaque->nomCourt);
    // Attaque
    TraceLog(LOG_INFO,"[attaquerPion] pionAttaque nom=%s pv=%d",pionAttaque->nomCourt,pionAttaque->pointsDeVie);
    if(pionQuiAttaque->attaque > pionAttaque->defense){
        pionAttaque->pointsDeVie = pionAttaque->pointsDeVie - (pionQuiAttaque->attaque - pionAttaque->defense) ;
    }
    if(pionQuiAttaque->attaque <= pionAttaque->defense){
        pionAttaque->pointsDeVie = pionAttaque->pointsDeVie - 1 ;
    }
    TraceLog(LOG_INFO,"[attaquerPion] pionAttaque nom=%s pv=%d",pionAttaque->nomCourt,pionAttaque->pointsDeVie);
    // Si le pion est mort on le met sur le cote
    if (pionAttaque->pointsDeVie <= 0) {
        if (pionAttaque->camp == CAMP_1) {
            deplacerPion(pionAttaque, nombreLignesGrille, pionAttaque->numeroPion);
        } else {
            deplacerPion(pionAttaque, nombreLignesGrille+1, pionAttaque->numeroPion);
        }
    } else {
        // Contra attaque
        if(pionAttaque->attaque > pionQuiAttaque->defense){
            pionQuiAttaque->pointsDeVie = pionQuiAttaque->pointsDeVie - (pionAttaque->attaque - pionQuiAttaque->defense) ;
        }
        if(pionAttaque->attaque <= pionQuiAttaque->defense){
            pionQuiAttaque->pointsDeVie = pionQuiAttaque->pointsDeVie - 1 ;
        }
        if (pionQuiAttaque->pointsDeVie <= 0) {
            if (pionQuiAttaque->camp == CAMP_1) {
                deplacerPion(pionQuiAttaque, nombreLignesGrille, pionQuiAttaque->numeroPion);
            } else {
                deplacerPion(pionQuiAttaque, nombreLignesGrille + 1, pionQuiAttaque->numeroPion);
            }
        }
    }
    TraceLog(LOG_INFO,"[attaquerPion] FIN");
}

void attaque(){
    if (!deplacementFait) return ; // Deplcement a faire d'abord
    if (IsKeyPressed(KEY_F1) || IsKeyPressed(KEY_LEFT_CONTROL)) { // Si touche F1 ou 'CTRL' appuyee alors pas d'attaque
        deplacementFait=false;
        // Changer de camp pour le prochain tour
        tourActuel = (tourActuel == 1) ? 2 : 1;
        return;
    }
    // Détection du clic de souris
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { // On designe le pion a attaquer
        Vector2 positionSouris = GetMousePosition();
        // Vérifier si un pion est clique et le sélectionner et si elle est pas dans la camp actuel
        for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
            if (CheckCollisionPointCircle(positionSouris, pionsGrille[i].position, TAILLE_CELLULE_GRILLE / 4) && pionsGrille[i].camp != tourActuel) {
                // Obtenir les indices de ligne et de colonne de la case cible
                int ligneCible = (GetMouseY() - DECALAGE_VERTICAL) / TAILLE_CELLULE_GRILLE;
                int colonneCible = (GetMouseX() - DECALAGE_HORIZONTAL) / TAILLE_CELLULE_GRILLE;
                TraceLog(LOG_INFO,"CALCUL CIBLE ligneCible=%d",ligneCible);
                TraceLog(LOG_INFO,"CALCUL CIBLE colonneCible=%d",colonneCible);
                // Vérifier si le déplacement est valide
                // pionSlection est le pion qui attaque
                pionGrille* pionAttaque = estAttaquePossible(ligneCible, colonneCible,pionsGrille);
                if (pionAttaque != NULL) {
                    // Attaquer pion
                    attaquerPion(denierPionSelectionne,pionAttaque);
                    // Changer de camp pour le prochain tour
                    tourActuel = (tourActuel == 1) ? 2 : 1;
                    deplacementFait=false; // On a fait attaque on peut de nouveau faire une mouvement
                }
                break;
            }
        }
    }
}
