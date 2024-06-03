/**************************************************************************
 * Nom du fichier : attaques.c
 * Description    : Contient toutes les fonbctions principales peremttant de gérére les attaques dans le jeu
 * Auteurs        : Team GEGK
 *                  Inspiré par https://fr.wikipedia.org/wiki/Distance_de_Manhattan pour les distances
 *								https://fr.wikipedia.org/wiki/Distance_de_Tchebychev
 * Historique     :
 *      1/05/2024 : Création initiale des fonctions
 *		23/05/2024 : Mise en place du fichier attaques.c 
 * Liste des fonctions :de   *		- attaque() : Fonction principale de l'attaque dans la boucle
 *		- estAttaquePossibleLigneColonne(porteeMax, ligneCourante, colonneCourante, ligneCible, colonneCible) : fonction indiquant si l'attaque est posible
 *		- estAttaquePossible(ligneCible, colonneCible, pions[]) : Fonction pour vérifier si une attaque est possible pour un pion donné
 *		- attaquerPion(pionQuiAttaque, pionAttaque) : Permet d'attaquer un pion
 **************************************************************************/ 

#include "attaques.h"

///////// ATTAQUES /////////////

int Tours = 1;

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
            TraceLog(LOG_TRACE, "[estAttaquePossibleLigneColonne] max=%d, dx=%d, dy=%d",porteeMax,deplacementX,deplacementY);
            resultat = !(deplacementX > porteeMax || deplacementY > porteeMax);
            break;
        case 2 : // Alorithme de Manhattan
            distance = abs(ligneCible - ligneCourante) + abs(colonneCible - colonneCourante);
            TraceLog(LOG_TRACE, "[estAttaquePossibleLigneColonne] max=%d, d=%d",porteeMax,distance);
            resultat = distance <= porteeMax;
            break;
        case 3 : // Alorithme de Tchebychev
            deplacementX = abs(ligneCourante - ligneCible);
            deplacementY = abs(colonneCourante - colonneCible);
            distance = deplacementX>deplacementY?deplacementX:deplacementY; // Il s'agit du MAX des 2
            TraceLog(LOG_TRACE, "[estAttaquePossibleLigneColonne] max=%d, dx=%d, dy=%d",porteeMax,deplacementX,deplacementX);
            resultat = distance <= porteeMax;
            break;
    }
    TraceLog(LOG_TRACE, "[estAttaquePossibleLigneColonne] Possible = %s",(resultat?"OUI":"NON"));
    return resultat;
}
// Fonction pour vérifier si une attaque est possible pour un pion donné
// Il renvoie le pion attaqué
pionGrille* estAttaquePossible(pionGrille* pionQuiAttaque,int ligneCible, int colonneCible,pionGrille pions[]) {
    // Vérifier si la case de destination est occupee par un autre pion
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
        if (pions[i].positionColonne == colonneCible && pions[i].positionLigne == ligneCible) {
            if (pions[i].camp != tourActuel) { // On peut attaque un pion que si dans l'autre camp
                if (pions[i].pointsDeVie > 0) { // On attaque pas les morts
                    if (estAttaquePossibleLigneColonne(pionQuiAttaque->portee,
                                                       pionQuiAttaque->positionLigne,
                                                       pionQuiAttaque->positionColonne, ligneCible,
                                                       colonneCible)) {
                        return &pions[i]; // Ligne non occupee par un pion et a portée donc on attaque
                    }
                }
            }
        }
    }
    return NULL; // Attaque impossible
}

// Indique si un pion peut en attaquer une autre
bool estAttaquePossiblePion(pionGrille* pionQuiAttaque,pionGrille* pionAttaque){
    if (pionQuiAttaque->camp == pionAttaque->camp) return false; // On n'attaque pas un pion de son camp
    if (pionQuiAttaque->pointsDeVie <=0 || pionAttaque->pointsDeVie <= 0) return false; // Ils sont morts
    if (estAttaquePossibleLigneColonne(pionQuiAttaque->portee,
                                       pionQuiAttaque->positionLigne,
                                       pionQuiAttaque->positionColonne, pionAttaque->positionLigne,
                                       pionAttaque->positionColonne)) {
        return true;
    }
    return false;
}

// Attaquer du pion
void attaquerPion(pionGrille* pionQuiAttaque,pionGrille* pionAttaque){
    TraceLog(LOG_TRACE,"[attaquerPion] DEBUT");
    TraceLog(LOG_TRACE,"[attaquerPion] pion %s attaque %s",pionQuiAttaque->nomCourt,pionAttaque->nomCourt);
    // Attaque
    TraceLog(LOG_TRACE,"[attaquerPion] Avant pionAttaque nom=%s pv=%d",pionAttaque->nomCourt,pionAttaque->pointsDeVie);
    if(pionQuiAttaque->attaque > pionAttaque->defense){
        pionAttaque->pointsDeVie = pionAttaque->pointsDeVie - (pionQuiAttaque->attaque - pionAttaque->defense) ;
    }
    if(pionQuiAttaque->attaque <= pionAttaque->defense){
        pionAttaque->pointsDeVie = pionAttaque->pointsDeVie - 1 ;
    }
    TraceLog(LOG_TRACE,"[attaquerPion] Après pionAttaque nom=%s pv=%d",pionAttaque->nomCourt,pionAttaque->pointsDeVie);
    // Si le pion est mort on le met sur le cote
    if (pionAttaque->pointsDeVie <= 0) {
        if (pionAttaque->camp == CAMP_1) {
            deplacerPion(pionAttaque, nombreLignesGrille, pionAttaque->numeroPion);
        } else {
            deplacerPion(pionAttaque, nombreLignesGrille+1, pionAttaque->numeroPion);
        }
    } else {
        // Contra attaque
        int ligneCourante = 0;
        int colonneCourante = 0;
        int ligneCible = 0;
        int colonneCible = 0;
        int distance = abs(ligneCible - ligneCourante) + abs(colonneCible - colonneCourante);
        if(pionAttaque->attaque > pionQuiAttaque->defense && pionAttaque->portee >= distance ){
            pionQuiAttaque->pointsDeVie = pionQuiAttaque->pointsDeVie - (pionAttaque->attaque - pionQuiAttaque->defense) ;
        }
        if(pionAttaque->attaque <= pionQuiAttaque->defense && pionAttaque->portee >= distance ){
            pionQuiAttaque->pointsDeVie = pionQuiAttaque->pointsDeVie - 1 ;
        }
        if (pionQuiAttaque->pointsDeVie <= 0) { // Si après contre attaque il est mort on le met hors de la grille
            if (pionQuiAttaque->camp == CAMP_1) {
                deplacerPion(pionQuiAttaque, nombreLignesGrille, pionQuiAttaque->numeroPion);
            } else {
                deplacerPion(pionQuiAttaque, nombreLignesGrille + 1, pionQuiAttaque->numeroPion);
            }
        }
        TraceLog(LOG_TRACE,"[Contre attaque] faites");
    }
    TraceLog(LOG_TRACE,"[attaquerPion] FIN");
}

// Fonction principale pour l'attaque
void attaque(){
    pionGrille *pionAttaque = NULL; // Pion qui attaque
    if (IsKeyPressed(KEY_D)) { affichePionsDebug(pionsGrille); } // POUR DEBUG
    if (!deplacementFait) return ; // Deplcement a faire d'abord - boucle infini il faut d'abord faire un mouvement
    if ((typeJeu == JEU_UN_JOUEUR && tourActuel == 2) || typeJeu == JEU_DEUX_IA) { // L'IA joue pour le camp 2 Attaque
        if (coupIAEnCours.pionAttaque >= 0) { // Attaque à faire
            pionAttaque = &pionsGrille[coupIAEnCours.pionAttaque];
            TraceLog(LOG_TRACE,"[attaquerIA] pion=%d",coupIAEnCours.pionAttaque);
            deplacementFait = false;
            // Changer de camp pour le prochain tour
            tourActuel = (tourActuel == 1) ? 2 : 1;
            denierPionSelectionne->estFatigue = true; // Le pion est fatigué
        }
        else{ // Pas d'attque uniquement mouvement
            deplacementFait = false;
            tourActuel = (tourActuel == 1) ? 2 : 1;
            denierPionSelectionne->estFatigue = true; // Le pion est fatigué
        }
    } else {
        if (IsKeyPressed(KEY_F1) ||
            IsKeyPressed(KEY_LEFT_CONTROL)) { // Si touche F1 ou 'CTRL' appuyee alors pas d'attaque
            deplacementFait = false;
            // Changer de camp pour le prochain tour
            Tours = Tours +1;
            tourActuel = (tourActuel == 1) ? 2 : 1;
            denierPionSelectionne->estFatigue = true; // Le pion est fatigué
            return;
        }
        // Détection du clic de souris
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { // On designe le pion a attaquer
            Vector2 positionSouris = GetMousePosition();
            // Vérifier si un pion est clique et le sélectionner et si elle est pas dans la camp actuel
            for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
                if (CheckCollisionPointCircle(positionSouris, pionsGrille[i].position, TAILLE_CELLULE_GRILLE / 4) &&
                    pionsGrille[i].camp != tourActuel) {
                    // Obtenir les indices de ligne et de colonne de la case cible
                    int ligneCible = (GetMouseY() - DECALAGE_VERTICAL) / TAILLE_CELLULE_GRILLE;
                    int colonneCible = (GetMouseX() - DECALAGE_HORIZONTAL) / TAILLE_CELLULE_GRILLE;
                    TraceLog(LOG_TRACE, "CALCUL CIBLE ligneCible=%d", ligneCible);
                    TraceLog(LOG_TRACE, "CALCUL CIBLE colonneCible=%d", colonneCible);
                    // Vérifier si le déplacement est valide
                    // pionSlection est le pion qui attaque
                    pionAttaque = estAttaquePossible(denierPionSelectionne,ligneCible, colonneCible, pionsGrille);
                    break;
                }
            }
        }
    }
    if (pionAttaque != NULL) {
        // Attaquer pion
        attaquerPion(denierPionSelectionne, pionAttaque);
        Tours = Tours +1;
        // Changer de camp pour le prochain tour
        tourActuel = (tourActuel == 1) ? 2 : 1;
        deplacementFait = false; // On a fait attaque on peut de nouveau faire une mouvement
        denierPionSelectionne->estFatigue = true; // Le pion est fatigué
    }
}
