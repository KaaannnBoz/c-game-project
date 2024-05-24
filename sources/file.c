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
