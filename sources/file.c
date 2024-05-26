/**************************************************************************
 * Nom du fichier : file.c
 * Description    : Gestion d'une file pour le parcours d'un arbre avec plusieurs fils pour l'IA est l'algo MinMAx
 * Auteurs        : Team GEGK
 *                  Inspiré par https://openclassrooms.com/fr/courses/19980-apprenez-a-programmer-en-c/19868-controlez-lajout-delements-avec-les-piles-et-les-files
 *                  Cours R. Gosswiller
 * Historique     :
 *      23/05/2024 : Création initiale du fichier.
 *      25/05/2024 : Premiers changements pour intégrer le jeu
 * Liste des fonctions :
 *      - void enfiler(File** tete, File** queue, Noeud* noeud) : mise en fil d'un noeud
 *      - Noeud* defiler(File** tete, File** queue) : sors de la file le noeud et libere la mémoire
 **************************************************************************/

#include <stdlib.h>
#include "file.h"

// Fonction pour ajouter un nœud à la file
void enfiler(File** tete, File** queue, Noeud* noeud) {
    File* nouveauElement = (File*)malloc(sizeof(File));
    nouveauElement->noeud = noeud;
    nouveauElement->suivant = NULL;
    if (*queue != NULL) {
        (*queue)->suivant = nouveauElement;
    }
    *queue = nouveauElement;
    if (*tete == NULL) {
        *tete = nouveauElement;
    }
}

// Fonction pour retirer un nœud de la file
Noeud* defiler(File** tete, File** queue) {
    if (*tete == NULL) {
        return NULL;
    }
    File* temp = *tete;
    Noeud* noeud = temp->noeud;
    *tete = (*tete)->suivant;
    if (*tete == NULL) {
        *queue = NULL;
    }
    free(temp);
    return noeud;
}
