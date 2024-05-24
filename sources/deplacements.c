#include "deplacements.h"

///////// DEPLACEMENTS /////////////

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
            TraceLog(LOG_INFO, "[estDeplacementPossibleLigneColonne] max=%d, dx=%d, dy=%d",deplacementMax,deplacementX,deplacementX);
            resultat = !(deplacementX > deplacementMax || deplacementY > deplacementMax);
            break;
        case 2 : // Algorithme de Manhattan
            distance = abs(ligneCible - ligneCourante) + abs(colonneCible - colonneCourante);
            TraceLog(LOG_INFO, "[estDeplacementPossibleLigneColonne] max=%d, d=%d",deplacementMax,distance);
            resultat = distance <= deplacementMax;
            break;
        case 3 : // Alorithme de Tchebychev
            deplacementX = abs(ligneCourante - ligneCible);
            deplacementY = abs(colonneCourante - colonneCible);
            distance = deplacementX>deplacementY?deplacementX:deplacementY; // Il s'agit du MAX des 2
            TraceLog(LOG_INFO, "[estDeplacementPossibleLigneColonne] max=%d, dx=%d, dy=%d",deplacementMax,deplacementX,deplacementX);
            resultat = distance <= deplacementMax;
            break;
    }
    TraceLog(LOG_INFO, "[estDeplacementPossibleLigneColonne] Possible = %s",(resultat?"OUI":"NON"));
    return resultat;
}
bool estCheminLibre(const pionGrille *pion, int ligneCible, int colonneCible,pionGrille pions[]) {
    int ligneCourante = pion->positionLigne;
    int colonneCourante = pion->positionColonne;
    int deltaLigne = ligneCible - ligneCourante;
    int deltaColonne = colonneCible - colonneCourante;
    int pasLigne = (deltaLigne > 0) ? 1 : -1;
    int pasColonne = (deltaColonne > 0) ? 1 : -1;
    int ligne, colonne;

    // Vérifie si le mouvement est horizontal, vertical ou diagonal
    if (deltaLigne == 0) { // Déplacement horizontal
        for (colonne = colonneCourante + pasColonne; colonne != colonneCible; colonne += pasColonne) {
            for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
                if (pions[i].positionLigne == ligneCourante && pions[i].positionColonne == colonne) {
                    return false; // Il y a un pion sur le chemin
                }
            }
        }
    } else if (deltaColonne == 0) { // Déplacement vertical
        for (ligne = ligneCourante + pasLigne; ligne != ligneCible; ligne += pasLigne) {
            for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
                if (pions[i].positionLigne == ligne && pions[i].positionColonne == colonneCourante) {
                    return false; // Il y a un pion sur le chemin
                }
            }
        }
    } else if (abs(deltaLigne) == abs(deltaColonne)) { // Déplacement diagonal
        for (ligne = ligneCourante + pasLigne, colonne = colonneCourante + pasColonne; ligne != ligneCible; ligne += pasLigne, colonne += pasColonne) {
            for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
                if (pions[i].positionLigne == ligne && pions[i].positionColonne == colonne) {
                    return false; // Il y a un pion sur le chemin
                }
            }
        }
    } else {
        return false; // Déplacement non autorisé
    }

    return true; // Le chemin est libre
}

// Fonction pour vérifier si un déplacement est possible pour un pion donné
bool estDeplacementPossible(const pionGrille *pion, int ligneCible, int colonneCible,pionGrille pions[]) {
    // Si elle est mort pas possible
    if (pion->pointsDeVie <=0) {
        return false; // Déplacement impossible (mort)
    }
    // Vérifier si on est dans la grille
    if (ligneCible < 0 || ligneCible >= NOMBRE_LIGNES_GRILLE || colonneCible < 0 || colonneCible >= NOMBRE_COLONNES_GRILLE) {
        return false; // Déplacement impossible (hors de la grille)
    }

    // Vérifier si le déplacement est possible selon la portée du pion
    if (!estDeplacementPossibleLigneColonne(pion->deplacement, pion->positionLigne, pion->positionColonne, ligneCible, colonneCible)) {
        return false; // Déplacement impossible
    }

    // Vérifier si la case de destination est occupée par un autre pion
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
        if (pions[i].positionColonne == colonneCible && pions[i].positionLigne == ligneCible) {
            return false; // Ligne occupée par un pion
        }
    }

    // Vérifier si le chemin est libre
    if (!estCheminLibre(pion, ligneCible, colonneCible,pions)) {
        return false; // Le chemin est bloqué par un autre pion
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

// Renvoie si un deplacement a été fait
void deplacement(){
    if (deplacementFait) return ; // Deplcement deja fait
    // Détection du clic de souris
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 positionSouris = GetMousePosition();

        // Vérifier si un pion est clique et le sélectionner
        for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
            if (CheckCollisionPointCircle(positionSouris, pionsGrille[i].position, TAILLE_CELLULE_GRILLE / 4) && pionsGrille[i].camp == tourActuel) {
                // Désélectionner le pion  sélectionné avant
                if (pionSelectionne != NULL) {
                    pionSelectionne->estSelectionne = false;
                }

                // Sélectionner le nouveau pion
                pionsGrille[i].estSelectionne = true;
                pionSelectionne = &pionsGrille[i]; // indique le nouveau pion selectionne
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
        deplacementPossible = estDeplacementPossible(pionSelectionne, ligneCible, colonneCible,pionsGrille);

        if (deplacementPossible) {
            // Déplacer le pion sélectionné
            deplacerPion(pionSelectionne, ligneCible, colonneCible);

            // Memoire du dernier pion selectionne (pour attaque)
            denierPionSelectionne = pionSelectionne;
            // Réinitialiser la sélection après le déplacement
            pionSelectionne->estSelectionne = false;
            pionSelectionne = NULL;

            // Changer de camp pour le prochain tour
            //tourActuel = (tourActuel == 1) ? 2 : 1;

            // Le nombre de coups a augmente
            nombreCoups++;
            deplacementFait=true; // Maiontenant attaque
        }
    }
}
