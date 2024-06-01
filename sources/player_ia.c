/**************************************************************************
 * Nom du fichier : player_ia.c
 * Description    : Contient les fonctions de simulation des joueurs (IA)
 *                  L'alorithme minimax utilisé est inspiré de wikipédia https://fr.wikipedia.org/wiki/Algorithme_minimax
 *                  Il a été écrit en C
 * Auteurs        : Team GEGK
 * Historique     :
 *      1/05/2024 : Création initiale du fichier (vide avec de fonction simulation faisan rien)
 *		23/05/2024 : Premier ajout d'un arbre pour test simple
 * Liste des fonctions :
 *		- void calculCoupIA(coupIA* coupEnCours,int ligneCible, int colonneCible,pionGrille pions[], int tourActuel): Calcul du cou
 *		- calculerMouvements : Calcul tous les mouvements et attaques
 *		- CalculerNoteTableau : calcul de la note pour chaque noeud
 *		- distancePions : calcul distance pour calculer note
 **************************************************************************/

#include "player_ia.h"
#include "raylib.h"
#include <stdlib.h>
#include <limits.h> // Utiliser pour + infini = INT_MAX et - infini = INT_MIN
#include <time.h>
#include "arbre.h"
#include "file.h"
#include "game.h"
#include "deplacements.h"
#include "attaques.h"

coupIA coupIAEnCours; // Coup en cours de l'IA
void calculerMouvements(Noeud* racine); // Calcul de tous les mouvements possibles
void CalculerNoteTableau(coupIA* coupEnfant); // Notation d'un tableau de pions
int distancePions(int x1,int y1,int x2, int y2); // Calcule de la distance entre deux pions
void calculerAttaques(Noeud* noeudPere,Noeud* noeudQuiAttaque); // Pour calculer toutes les attaque possible depuis un noeud
void calculerAttaquesPlusieursNiveaux(Noeud* noeudPere,Noeud* noeudQuiAttaque,int profondeur, int profondeur_max);


// NOTATION des noeus //
// Foncrion de notation
void CalculerNoteTableau(coupIA* coupEnfant){
    coupEnfant->note = 0;
    int nombre_pions_camp_ami = 0;
    int points_vie_totaux_camp_ami = 0;
    int nombre_pions_camp_ennemi = 0;
    int points_vie_totaux_camp_ennemi = 0;
    int proximite_pions_amis_entre_eux = 0;
    int proximite_pions_amis_et_ennemis = 0;
    for (int i=0; i< NOMBRE_PIONS_MAX;i++){
        if (coupEnfant->campPion == coupEnfant->tableauPions[i].camp) { // Ami
            if (coupEnfant->tableauPions[i].pointsDeVie > 0) { // On ne compte que les vivants
                // Nombre de pions dans mon camp encore en vie +1000 par pion dans le camp qui joue
                nombre_pions_camp_ami += 1000;
                // Point de vie totaux dans mon camp +20 par point de vie
                points_vie_totaux_camp_ami += (coupEnfant->tableauPions[i].pointsDeVie * 20);
                // Calcul des proximites des pions amis entre eux
                for (int j=0; j< NOMBRE_PIONS_MAX;j++){
                    if (coupEnfant->campPion == coupEnfant->tableauPions[j].camp){ // Dan mon camp
                        if (coupEnfant->tableauPions[j].pointsDeVie > 0){ // Les vivants
                            if (j > i){ // Pas pour le pion avec lui meme, et on fait pas 2 fis la distance
                                // P1 avec P2, P3 et P4
                                // P2 avec P3 et P4
                                // P4 avec rien
                                int x1= coupEnfant->tableauPions[i].positionLigne;
                                int y1= coupEnfant->tableauPions[i].positionColonne;
                                int x2= coupEnfant->tableauPions[j].positionLigne;
                                int y2= coupEnfant->tableauPions[j].positionColonne;
                                // -1 par cas de distance
                                proximite_pions_amis_entre_eux += (distancePions(x1,y1,x2,y2)*(-1));
                            }
                        }
                    } else {
                        if (coupEnfant->tableauPions[j].pointsDeVie > 0){ // Les vivants
                            if (j > i){ // Pas pour le pion avec lui meme, et on fait pas 2 fis la distance
                                // P1 avec P2, P3 et P4
                                // P2 avec P3 et P4
                                // P4 avec rien
                                int x1= coupEnfant->tableauPions[i].positionLigne;
                                int y1= coupEnfant->tableauPions[i].positionColonne;
                                int x2= coupEnfant->tableauPions[j].positionLigne;
                                int y2= coupEnfant->tableauPions[j].positionColonne;
                                // -1 par cas de distance
                                proximite_pions_amis_et_ennemis += (distancePions(x1,y1,x2,y2)*(+1));
                            }
                        }
                    }
                }
            }
        } else { // ennemi
            if (coupEnfant->tableauPions[i].pointsDeVie > 0) {
                // points de vie totaux chez l'ennemei -10 par point de vie
                points_vie_totaux_camp_ennemi -= (coupEnfant->tableauPions[i].pointsDeVie * 10);
                // Nombre de pions dans le camp ennemei enore en vie -500 par pion
                nombre_pions_camp_ennemi -= 500;
            }
        }
    }
    // Calcul final de la note
    coupEnfant->note =  nombre_pions_camp_ami+points_vie_totaux_camp_ami+
                        nombre_pions_camp_ennemi+points_vie_totaux_camp_ennemi+
                        proximite_pions_amis_entre_eux + proximite_pions_amis_et_ennemis;
}

// Algorithmes permettant de calcluler la distance entre deux cases
// Pour la distance Mannathan voir Wikipedia ==> https://fr.wikipedia.org/wiki/Distance_de_Manhattan
// Il existe aussi la distance Tchebychev ==> de https://fr.wikipedia.org/wiki/Distance_de_Tchebychev
int distancePions(int x1,int y1,int x2, int y2){
    int distanceX = 0;
    int distanceY = 0;
    int distance = 0;
    int algo = 2; // Pour l'instant on utilise Manhattan
    switch (algo){
        case 1 : // Classique (PAS POSSIBLE)
            distanceX = abs(x1 - x2);
            distanceY = abs(y1 - y2);
            TraceLog(LOG_TRACE, "[distancePions] distance=%d, dx=%d, dy=%d",distance, distanceX,distanceY);
            break;
        case 2 : // Algorithme de Manhattan
            distanceX = abs(x1 - x2);
            distanceY = abs(y1 - y2);
            distance = distanceX + distanceY;
            TraceLog(LOG_TRACE, "[distancePions Manhattan] distance=%d, dx=%d, dy=%d",distance,distanceX,distanceX);
            break;
        case 3 : // Alorithme de Tchebychev
            distanceX = abs(x1 - x2);
            distanceY = abs(y1 - y2);
            distance = distanceX>distanceY?distanceX:distanceY; // Il s'agit du MAX des 2
            TraceLog(LOG_TRACE, "[distancePions] distance=%d, dx=%d, dy=%d",distance,distanceX,distanceX);
            break;
    }
    return distance;
}

// NOTATION des noeuds //

// MULTI NIVEAUX

void calculerMouvementsPlusieursNiveaux(Noeud* racine,int profondeur, int profondeur_max){
    TraceLog(LOG_TRACE,"[==>calculerMouvements]");
	if (profondeur >= profondeur_max) {
        racine->est_terminal = true;
        racine->note = racine->coup.note;
        return;
    }
	// Calcul de tous les enfants possibles
    for (int pion=0;pion <NOMBRE_PIONS_MAX;pion++) {
        // On prend que les pions vivant du camp en cours
        if (racine->coup.tableauPions[pion].camp == racine->coup.campPion && racine->coup.tableauPions[pion].pointsDeVie > 0) {
            for (int i = 0; i < nombreLignesGrille; i++) {
                for (int j = 0; j < nombreColonnesGrille; j++) {
                    TraceLog(LOG_TRACE, "[calculerMouvements] Deplacement possible ? %d,%d,%s ", i, j,racine->coup.tableauPions[i].nomCourt);
                    if (estDeplacementPossible(&racine->coup.tableauPions[pion], i, j, racine->coup.tableauPions)) {
                        TraceLog(LOG_TRACE, "[calculerMouvements] DEPLACEMENT  POSSIBLE %d,%d ", i, j);
						Noeud* enfant;
                        coupIA coupEnfant;
                        coupEnfant.campPion = racine->coup.campPion;
                        coupEnfant.pionEnMouvement = pion;
                        coupEnfant.pionAttaque = -1; // Pas d'attaque -1 que un mouvement
                        coupEnfant.colonneCibleMouvement = j;
                        coupEnfant.ligneCibleMouvement = i;
                        for (int pionC = 0; pionC < NOMBRE_PIONS_MAX; pionC++) { // Copie du tableau
                            coupEnfant.tableauPions[pionC] = racine->coup.tableauPions[pionC];
                            TraceLog(LOG_TRACE,"[calculerMouvements][%d] nomCourt=%s",pionC,coupEnfant.tableauPions[pion].nomCourt);
                        }
                        deplacerPion(&coupEnfant.tableauPions[pion], coupEnfant.ligneCibleMouvement,
                                     coupEnfant.colonneCibleMouvement);
                        coupEnfant.tableauPions[pion].estFatigue = true;
                        resetPionsFatigues(coupEnfant.tableauPions); // Dans la simulation on remet tous les pions à non fatigue.
                        CalculerNoteTableau(&coupEnfant);
                        enfant = creerNoeud(coupEnfant);
                        TraceLog(LOG_TRACE,"[calculerMouvements][%d,%d] nomCourt=%s, colCible=%d, ligneCible=%d, colEnCOurs=%d, ligneEncours=%d",
                                 pion,coupEnfant.pionEnMouvement,coupEnfant.tableauPions[pion].nomCourt,
                                 coupEnfant.colonneCibleMouvement, coupEnfant.ligneCibleMouvement,
                                 coupEnfant.tableauPions[i].positionColonne,coupEnfant.tableauPions[i].positionLigne
                                 );
                        ajouterEnfant(racine, enfant);
						// On continue sur plusieurs niveaux
						calculerMouvementsPlusieursNiveaux(enfant,profondeur+1,profondeur_max);
                        // On peut faire toutes les attaques possibles
                        calculerAttaquesPlusieursNiveaux(racine,enfant,profondeur,profondeur_max);
                    }
                }
            }
        }
    }
    TraceLog(LOG_TRACE,"[<==calculerMouvements]");
}

void calculerAttaquesPlusieursNiveaux(Noeud* noeudPere,Noeud* noeudQuiAttaque,int profondeur, int profondeur_max) {
    TraceLog(LOG_TRACE,"[==>calculerAttaquesPlusieursNiveaux]");
    Noeud* enfant;
    int pionQuiAttaque;
    for (int pion=0;pion <NOMBRE_PIONS_MAX;pion++) { // Tous les pions attaquer
        pionQuiAttaque = noeudQuiAttaque->coup.pionEnMouvement;
        if (estAttaquePossiblePion(&noeudQuiAttaque->coup.tableauPions[pionQuiAttaque],
                                   &noeudQuiAttaque->coup.tableauPions[pion])) {
            coupIA coupEnfant = noeudQuiAttaque->coup; // Copie du coup
            coupEnfant.pionAttaque = pion; // En plus un mouvement
            attaquerPion(&coupEnfant.tableauPions[pionQuiAttaque], &coupEnfant.tableauPions[pion]);
            CalculerNoteTableau(&coupEnfant);
            enfant = creerNoeud(coupEnfant);
            /*TraceLog(LOG_TRACE, "[calculerAttaques] Pion=%s attaque %s",
                     coupEnfant.tableauPions[pionQuiAttaque].nomCourt,coupEnfant.tableauPions[pion].nomCourt);*/
            ajouterEnfant(noeudPere, enfant);
			// On continue sur plusieurs niveaux
			calculerMouvementsPlusieursNiveaux(enfant,profondeur+1,profondeur_max);
        }
    }
    TraceLog(LOG_TRACE,"[<==calculerAttaquesPlusieursNiveaux]");
}
// Fonction principale permettant de generer l'arbre des coups possibles
void calculCoupIA(coupIA* coupEnCours,pionGrille pions[], int tourActuel) {
    SetTraceLogLevel(LOG_INFO); // Pour enlever les traces car en mode IA il y a des traces partouts
    Noeud* noeudMaxScore;
    // calculer tous les mouvements possibles
    coupEnCours->campPion = tourActuel;
    for (int pion=0;pion <NOMBRE_PIONS_MAX;pion++) { // Copie du tableau
        coupEnCours->tableauPions[pion] = pions[pion];
        //TraceLog(LOG_TRACE,"[calculCoupIA][%d] nomCourt=%s",pion,coupEnCours->tableauPions[pion].nomCourt);
    }
    Noeud* racine = creerNoeud(*coupEnCours);
    // Calcul des mouvements et des attaques possibles
    calculerMouvementsPlusieursNiveaux(racine,0,optionsIA);
    TraceLog(LOG_INFO, "[==>afficherArbre]");
    afficherArbre(racine,1);
    TraceLog(LOG_INFO, "[<==afficherArbre]");
    TraceLog(LOG_INFO, "[<==minimax]");
    // Evaluation du meilleur coup
    noeudMaxScore = lancerMinimax(racine);
    TraceLog(LOG_INFO, "[==>minimax]");
    if (noeudMaxScore != NULL){
        afficherNoeud(noeudMaxScore);
        coupEnCours->colonneCibleMouvement = noeudMaxScore->coup.colonneCibleMouvement;
        coupEnCours->ligneCibleMouvement = noeudMaxScore->coup.ligneCibleMouvement;
        coupEnCours->pionEnMouvement = noeudMaxScore->coup.pionEnMouvement;
        coupEnCours->pionAttaque = noeudMaxScore->coup.pionAttaque;
    } else {
        TraceLog(LOG_INFO, "[calculCoupIA] COUP IMPOSSIBLE");
    }
    libererArbre(racine);
    SetTraceLogLevel(LOG_TRACE); // Revenir aux traces normales
}