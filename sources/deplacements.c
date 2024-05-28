/**************************************************************************
 * Nom du fichier : deplacements.c
 * Description    : Contient toutes les fonbctions  permettant de gerer les déplacements des pions dans le jeu
 * Auteurs        : Team GEGK
 *                  Inspiré par https://fr.wikipedia.org/wiki/Distance_de_Manhattan pour les distances
 *								https://fr.wikipedia.org/wiki/Distance_de_Tchebychev
 * Historique     :
 *      1/5/2024 : Création initiale des fonctions
 *		23/5/2024 : Mise en place du fichier deplacements.c 
 *		25/5/2024 : Ajout de l'IA un joueur 
 *					 
 * Liste des fonctions :
 * 	- estDeplacementPossibleLigneColonne(deplacementMax, ligneCourante, colonneCourante, ligneCible, colonneCible)
 * 	- estCheminLibre(pion, ligneCible, colonneCible, pions[]) : permet de s'assurer que l'on ne saute pâs de pion dans un chemin
 * 	- estDeplacementPossible(pion, ligneCible, colonneCible, pions[]) : indique si on pion peut aller à une destination donnée
 * 	- deplacerPion(pion, ligneCible, colonneCible) : effectue le déplacement du pion 
 * 	- deplacement() : fonction principale de la boucle de jeu - gestion du déplacement
 **************************************************************************/

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
            //TraceLog(LOG_INFO, "[estDeplacementPossibleLigneColonne] max=%d, d=%d",deplacementMax,distance);
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
    //TraceLog(LOG_INFO, "[estDeplacementPossibleLigneColonne] Possible = %s",(resultat?"OUI":"NON"));
    return resultat;
}
// Indique si le pion qui veut bouger va sauter un autre pion. il ne peut pas
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
    // Si le pion est fatigué il ne peut pas bouger
    if (pion->estFatigue) {
        return false; // Déplacement impossible (fatigué)
    }    
    // Si elle est mort pas possible
    if (pion->pointsDeVie <=0) {
        return false; // Déplacement impossible (mort)
    }
    // Vérifier si on est dans la grille
    if (ligneCible < 0 || ligneCible >= nombreLignesGrille || colonneCible < 0 || colonneCible >= nombreColonnesGrille) {
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
    TraceLog(LOG_INFO, "[deplacerPion] nom=%s,ligne=%d, colonne=%d ligneCible=%d, colonneCible=%d",pion->nomCourt,pion->positionLigne, pion->positionColonne,ligneCible,colonneCible);
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
	int ligneCible, colonneCible;
    bool faireDeplacement = false; // Indique si on fait le depalcment
    if (IsKeyPressed(KEY_D)) { affichePionsDebug(pionsGrille); } // POUR DEBUG
    if (deplacementFait) return ; // Deplcement deja fait
	// IA DOIT JOUER
	if ((typeJeu == JEU_UN_JOUEUR && tourActuel == 2) || typeJeu == JEU_DEUX_IA) { // L'IA joue pour le camp 2
		calculCoupIA(&coupIAEnCours,pionsGrille,tourActuel); // DEMARRAGE DE L'IA
		deplacementPossible = true;
        faireDeplacement = true;
		ligneCible = coupIAEnCours.ligneCibleMouvement;
		colonneCible = coupIAEnCours.colonneCibleMouvement;
		pionSelectionne = &pionsGrille[coupIAEnCours.pionEnMouvement];

	} else {
		// DEPLACEMENT SANS IA
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
			ligneCible = (GetMouseY() - DECALAGE_VERTICAL) / TAILLE_CELLULE_GRILLE;
			colonneCible = (GetMouseX() - DECALAGE_HORIZONTAL) / TAILLE_CELLULE_GRILLE;
			TraceLog(LOG_INFO,"CALCUL ligneCible=%d",ligneCible);
			TraceLog(LOG_INFO,"CALCUL colonneCible=%d",colonneCible);
			// Vérifier si le déplacement est valide
            faireDeplacement = estDeplacementPossible(pionSelectionne, ligneCible, colonneCible,pionsGrille);
            deplacementPossible = faireDeplacement;
		}
	}
	// On effectue le deplacement
	if (faireDeplacement) {
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
		deplacementPossible = true;
	}
}

// Remet tous les pions fatigués à non fatigués 
void resetPionsFatigues(pionGrille pions[]){
    int PionsFatiguesC1 = 0;
    int PionsFatiguesC2 = 0;
    
    for (int i = 0; i < NOMBRE_PIONS_MAX; i++) { // Compte les pions fatigués
        if (pionsGrille[i].camp == CAMP_2) {
            if (pionsGrille[i].estFatigue || pionsGrille[i].pointsDeVie <=0 ) PionsFatiguesC2++;
        }
        else {
            if (pionsGrille[i].estFatigue || pionsGrille[i].pointsDeVie <=0 ) PionsFatiguesC1++;
        }
    }
    // Si tous les pions sont fatigues dans un camp alors non fatigués (divise par 2 car il y autant de pions dans le camps que dans camp 2)
    if (PionsFatiguesC1 >= NOMBRE_PIONS_MAX/2 || PionsFatiguesC2 >= NOMBRE_PIONS_MAX/2){ 
        for (int i = 0; i < NOMBRE_PIONS_MAX; i++) {
                if (pionsGrille[i].camp == CAMP_1 && PionsFatiguesC1 >= NOMBRE_PIONS_MAX/2 ) pionsGrille[i].estFatigue = false;
                if (pionsGrille[i].camp == CAMP_2 && PionsFatiguesC2 >= NOMBRE_PIONS_MAX/2 ) pionsGrille[i].estFatigue = false;
            }   
    }
}