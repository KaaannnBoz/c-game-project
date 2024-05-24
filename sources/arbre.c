#include <stdio.h>
#include <stdlib.h>
#include "arbre.h"
#include "file.h"

// Fonction pour créer un nouveau nœud
Noeud* creerNoeud(int donnee) {
    Noeud* nouveauNoeud = (Noeud*)malloc(sizeof(Noeud));
    nouveauNoeud->donnee = donnee;
    nouveauNoeud->enfants = NULL;
    nouveauNoeud->nbEnfants = 0;
    return nouveauNoeud;
}

// Fonction pour ajouter un enfant à un nœud
void ajouterEnfant(Noeud* parent, Noeud* enfant) {
    parent->nbEnfants++;
    parent->enfants = (Noeud**)realloc(parent->enfants, parent->nbEnfants * sizeof(Noeud*));
    parent->enfants[parent->nbEnfants - 1] = enfant;
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
        printf("%d\n", racine->donnee);
        for (int i = 0; i < racine->nbEnfants; i++) {
            afficherArbre(racine->enfants[i], niveau + 1);
        }
    }
}

// Fonction pour parcourir l'arbre en largeur
void parcoursLargeur(Noeud* racine) {
    if (racine == NULL) {
        return;
    }
    
    File *tete = NULL, *queue = NULL;
    enfiler(&tete, &queue, racine);
    
    while (tete != NULL) {
        Noeud* courant = defiler(&tete, &queue);
        printf("%d ", courant->donnee);
        
        for (int i = 0; i < courant->nbEnfants; i++) {
            enfiler(&tete, &queue, courant->enfants[i]);
        }
    }
    printf("\n");
}


