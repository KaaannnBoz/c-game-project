/**************************************************************************
 * Nom du fichier : arbre.c
 * Description    : Gestion d'un arbre avec plusieurs fils pour l'IA est l'algo MinMAx
 *                  L'arbre gérer est un arbre avec plusieurs fils
 * Auteurs        : Team GEGK
 *                  Inspiré par https://fr.wikipedia.org/wiki/%C3%89lagage_alpha-b%C3%AAta
 *                  Inspiré par https://openclassrooms.com/forum/sujet/creation-arbre-n-aire
 *                  Inspirée par https://fr.wikipedia.org/wiki/Algorithme_minimax avec le Pseudocode 
 *                  Cours R. Gosswiller
 * Historique     :
 *      23/05/2024 : Création initiale du fichier.
 *      25/05/2024 : Premiers changements pour intégrer le jeu
 *      26/5/2024 : Ajout IA calcule de tous les coups
 *      28 et 29/5: Ajout de deux IA et minimax
 * Liste des fonctions :
 *      - Noeud* creerNoeud(int donnee) : Création d'un noeud 
 *      - ajouterEnfant(Noeud* parent, Noeud* enfant) : ajout d'un enfant au noeud
 *      - libererArbre(Noeud* racine) : liébération de la mémoire (free)
 *      - afficherArbre(Noeud* racine, int niveau) : affichage de l'arbre pour tests
 *      - Noeud* lancerMinimax(Noeud* racine) : Lancement minimax et renvoie le meilleur noeud
 *      - int minimax(Noeud* noeud, int profondeur, int alpha, int beta, bool noeudTypeMax) : algorithme minimax 
 *      - Noeud* obtenirParent(Noeud* noeud)  : Récup du parent pour connaitre le coup
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "arbre.h"
#include "file.h"

// Fonction pour créer un nouveau noeud

Noeud* creerNoeud(coupIA unCoup) {
    Noeud* nouveauNoeud = (Noeud*)malloc(sizeof(Noeud));
    nouveauNoeud->coup = unCoup;
    nouveauNoeud->enfants = NULL;
    nouveauNoeud->nbEnfants = 0;
    nouveauNoeud->parent = NULL;
    nouveauNoeud->note = 0;
    nouveauNoeud->est_terminal = false;
    return nouveauNoeud;
}

// Fonction pour ajouter un enfant à un noeud
void ajouterEnfant(Noeud* parent, Noeud* enfant) {
    parent->nbEnfants++;
    parent->enfants = (Noeud**)realloc(parent->enfants, parent->nbEnfants * sizeof(Noeud*));
    parent->enfants[parent->nbEnfants - 1] = enfant;
    enfant->parent = parent; // Définir le parent de l'enfant
}

// Fonction pour obtenir le parent d'un noeud
Noeud* obtenirParent(Noeud* noeud) {
    return noeud->parent;
}

// Fonction pour libérer la mémoire allouée à l'arbre
void libererArbre(Noeud* racine) {
    if (racine != NULL) {
        for (int i = 0; i < racine->nbEnfants; i++) {
            libererArbre(racine->enfants[i]);
        }
        free(racine->enfants);
        free(racine);
    }
}

// Fonction pour afficher l'arbre
void afficherArbre(Noeud* racine, int niveau) {
    if (racine != NULL) {
        for (int i = 0; i < niveau; i++) {
            printf("  ");
        }
        printf("[%d] no=%d,nom=%s, note=%d, camp=%d, posL=%d, posC=%d, attaque=%d \n",niveau,racine->coup.pionEnMouvement,
                                racine->coup.tableauPions[racine->coup.pionEnMouvement].nomCourt,
                                racine->coup.note,
                                racine->coup.tableauPions[racine->coup.pionEnMouvement].camp,
                                racine->coup.tableauPions[racine->coup.pionEnMouvement].positionLigne,
                                racine->coup.tableauPions[racine->coup.pionEnMouvement].positionColonne,
                                racine->coup.pionAttaque);
        for (int i = 0; i < racine->nbEnfants; i++) {
            afficherArbre(racine->enfants[i], niveau + 1);
        }
    }
}

void afficherNoeud(Noeud* noeud) {
    TraceLog(LOG_TRACE, "[==>afficherNoeud]");
    printf("no=%d,nom=%s, note=%d, camp=%d, posL=%d, posC=%d, attaque=%d \n",      noeud->coup.pionEnMouvement,
                                noeud->coup.tableauPions[noeud->coup.pionEnMouvement].nomCourt,
                                noeud->coup.note,
                                noeud->coup.tableauPions[noeud->coup.pionEnMouvement].camp,
                                noeud->coup.tableauPions[noeud->coup.pionEnMouvement].positionLigne,
                                noeud->coup.tableauPions[noeud->coup.pionEnMouvement].positionColonne,
                                noeud->coup.tableauPions[noeud->coup.pionEnMouvement].attaque);
     TraceLog(LOG_TRACE, "[<==afficherNoeud]");
}

// Fonction principale Minimax utilisant BFS 
// On renvoie le noeud qui doit jouer
// NON UTILISE
Noeud* minimaxLargeurDeux(Noeud* racine) {
    if (racine == NULL) {
        return NULL;
    }
    File* tete = NULL;
    File* queue = NULL;
    enfiler(&tete, &queue, racine); // Enfiler la racine

    int niveau = 0;
    int meilleurNote = INT_MIN; // Pour la maximisation
    Noeud* meilleurNoeud = NULL;

    while (tete != NULL) {
        Noeud* noeudCourant = defiler(&tete, &queue); // Défiler un nœud
        if (noeudCourant == NULL) {
            // Si le noeud courant est NULL, onb change de niveau
            niveau++;
            continue;
        }

        // Pour les niveaux 2 et 4 on fait le max (nivau pairs %2 == 0)
        if (niveau % 2 == 0) { // Pour les niveaux pair (MAX)
            if (noeudCourant->note > meilleurNote) {
                meilleurNote = noeudCourant->note;
                meilleurNoeud = noeudCourant;
            }
        } else { // Pour les niveaux impairs (MIN)
            if (noeudCourant->note < meilleurNote) {
                meilleurNote = noeudCourant->note;
                meilleurNoeud = noeudCourant;
            }
        }
        // Ajouter les enfants du noeud courant à la file
        for (int i = 0; i < noeudCourant->nbEnfants; i++) {
            enfiler(&tete, &queue, noeudCourant->enfants[i]);
        }
        if (tete != NULL) {
            enfiler(&tete, &queue, NULL); // Marqueur de niveau
        }
    }
    return meilleurNoeud;
}


//
// Algorithme minimax https://fr.wikipedia.org/wiki/Algorithme_minimax
// L'algoritme utilisé est C https://fr.wikipedia.org/wiki/%C3%89lagage_alpha-b%C3%AAta
// la base est sur https://fr.wikipedia.org/wiki/Algorithme_minimax
// On utilise Pseudocode
//

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

int minimax(Noeud* noeud, int profondeur, int alpha, int beta, bool noeudTypeMin) {
    int v;
    if (profondeur == 0 || noeud->est_terminal) { // On est sur une feuille
        return noeud->note; // Retourne la note (calculer avant avec CalculerNoteTableau)
    }
    if (noeudTypeMin) { // noeud de type MIN
        v = INT_MIN;
        for (int i = 0; i < noeud->nbEnfants; i++) {
            int eval = minimax(noeud->enfants[i], profondeur - 1, alpha, beta, false);
            v = max(v, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) {
                break; // elage alpha-beta
            }
        }
        return v;
    } else {
        v = INT_MAX; // noeud de type MAX
        for (int i = 0; i < noeud->nbEnfants; i++) {
            int eval = minimax(noeud->enfants[i], profondeur - 1, alpha, beta, true);
            v = min(v, eval);
            beta = min(beta, eval);
            if (beta <= alpha) {
                break; // elage alpha-beta
            }
        }
        return v;
    }
}

// Algorithme minimax 
Noeud* lancerMinimax(Noeud* racine) {
    int profondeur = optionsIA; // Profondeur de recherche définie par les options de l'IA
    int maxEval = INT_MIN; // Par defaut on met moins infini
    Noeud* meilleurNoeud = NULL; // Pas de maielleur noeud

    // Parcours des enfants de la racine (niveau 2) pour calculer ses enfants
    for (int i = 0; i < racine->nbEnfants; i++) {
        int eval = minimax(racine->enfants[i], profondeur - 1, INT_MIN, INT_MAX, false);
        if (eval > maxEval) {
            maxEval = eval;
            meilleurNoeud = racine->enfants[i];
        }
    }

    // Si aucun meilleur noeud n'est trouvé, renvoyer le meilleur coup de niveau 1
    // Cas possible si uniquement sur 3 niveaux
    // Evite les coups impossibles
    if (meilleurNoeud == NULL) {
        for (int i = 0; i < racine->nbEnfants; i++) {
            if (racine->enfants[i]->note > maxEval) {
                maxEval = racine->enfants[i]->note;
                meilleurNoeud = racine->enfants[i];
            }
        }
    }

    return meilleurNoeud;
}