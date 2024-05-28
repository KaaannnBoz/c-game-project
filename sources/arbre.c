/**************************************************************************
 * Nom du fichier : arbre.c
 * Description    : Gestion d'un arbre avec plusieurs fils pour l'IA est l'algo MinMAx
 *                  L'arbre gérer est un arbre avec plusieurs fils
 * Auteurs        : Team GEGK
 *                  Inspiré par https://openclassrooms.com/forum/sujet/creation-arbre-n-aire
 *                  Inspiré par https://fr.wikipedia.org/wiki/Algorithme_de_parcours_en_largeur
 *                  Inspriré par https://www.youtube.com/watch?v=z1K6YMJmr6s&t=482s
 *                  Cours R. Gosswiller
 * Historique     :
 *      23/05/2024 : Création initiale du fichier.
 *      25/05/2024 : Premiers changements pour intégrer le jeu
 *      26/5/2024 : Ajout IA calcule de tous les coups
 * Liste des fonctions :
 *      - Noeud* creerNoeud(int donnee) : Création d'un noeud 
 *      - ajouterEnfant(Noeud* parent, Noeud* enfant) : ajout d'un enfant au noeud
 *      - libererArbre(Noeud* racine) : liébération de la mémoire (free)
 *      - afficherArbre(Noeud* racine, int niveau) : affichage de l'arbre pour tests
 *      - void parcoursLargeur(Noeud* racine) : Parcours en largeur de l'arbre (alog avec files)
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
    nouveauNoeud->parent = NULL; // Initialiser le parent à NULL
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
        printf("no=%d,nom=%s,posL=%d,posC=%d \n",      racine->coup.pionEnMouvement,
                                racine->coup.tableauPions[racine->coup.pionEnMouvement].nomCourt,
                                racine->coup.tableauPions[racine->coup.pionEnMouvement].positionLigne,
                                racine->coup.tableauPions[racine->coup.pionEnMouvement].positionColonne);
        for (int i = 0; i < racine->nbEnfants; i++) {
            afficherArbre(racine->enfants[i], niveau + 1);
        }
    }
}

// Fonction pour parcourir l'arbre en largeur
/*
Noeud* parcoursLargeur(Noeud* racine) {
    Noeud* pionMaxScore = NULL;

    if (racine == NULL) {
        return;
    }
    File *tete = NULL, *queue = NULL;
    enfiler(&tete, &queue, racine);
    
    while (tete != NULL) {
        Noeud* courant = defiler(&tete, &queue);

        printf("no=%d,nom=%s,posL=%d,posC=%d SCORE=%d, cibleL=%d,cibleC=%d\n", courant->coup.pionEnMouvement,
               courant->coup.tableauPions[courant->coup.pionEnMouvement].nomCourt,
               courant->coup.tableauPions[courant->coup.pionEnMouvement].positionLigne,
               courant->coup.tableauPions[courant->coup.pionEnMouvement].positionColonne,
               courant->coup.note,courant->coup.ligneCibleMouvement,courant->coup.colonneCibleMouvement);
        if (pionMaxScore == NULL) {
            pionMaxScore = courant;
        } else {
            if (courant->coup.note > pionMaxScore->coup.note) {
                pionMaxScore = courant;
            }
        }
        for (int i = 0; i < courant->nbEnfants; i++) {
            enfiler(&tete, &queue, courant->enfants[i]);
        }
    }
    printf("MAX no=%d,nom=%s,posL=%d,posC=%d SCORE=%d, cibleL=%d,cibleC=%d\n", pionMaxScore->coup.pionEnMouvement,
           pionMaxScore->coup.tableauPions[pionMaxScore->coup.pionEnMouvement].nomCourt,
           pionMaxScore->coup.tableauPions[pionMaxScore->coup.pionEnMouvement].positionLigne,
           pionMaxScore->coup.tableauPions[pionMaxScore->coup.pionEnMouvement].positionColonne,
           pionMaxScore->coup.note,pionMaxScore->coup.ligneCibleMouvement,pionMaxScore->coup.colonneCibleMouvement);
    return pionMaxScore;
}*/

// Parcours de l'arbre en largeur pour calcul du min et du max
// pour arbre à une seul niveao on calcul le niveau 1
// Fait le max pour l'instant
Noeud* parcoursLargeur(Noeud* racine) {
    if (racine == NULL) {
        return NULL;
    }

    Noeud* pionMaxScore = NULL;
    File *tete = NULL, *queue = NULL;
    enfiler(&tete, &queue, racine);
    enfiler(&tete, &queue, NULL);  // Marqueur de niveau

    int niveau = 0;
    int maxNote = INT_MIN;
    int minNote = INT_MAX;

    while (tete != NULL) {
        Noeud* courant = defiler(&tete, &queue);

        if (courant == NULL) {
            printf("Niveau %d: Max Note = %d, Min Note = %d\n", niveau, maxNote, minNote);

            // Réinitialiser les valeurs pour le prochain niveau
            maxNote = INT_MIN;
            minNote = INT_MAX;

            // Si la file n'est pas vide, ajouter un nouveau marqueur de niveau
            if (tete != NULL) {
                enfiler(&tete, &queue, NULL);
            }
            niveau++;
            continue;
        }

        printf("Niveau %d, no=%d, nom=%s, NOTE=%d, cibleL=%d, cibleC=%d, pionAttaque=%d\n",
               niveau, courant->coup.pionEnMouvement,
               courant->coup.tableauPions[courant->coup.pionEnMouvement].nomCourt,
               courant->coup.note,
               courant->coup.ligneCibleMouvement,
               courant->coup.colonneCibleMouvement,
               courant->coup.pionAttaque);

        if (courant->coup.note > maxNote && niveau>0) { // Si la note est plus grande on le memorise
            maxNote = courant->coup.note;
            pionMaxScore = courant;
        }
        if (courant->coup.note < minNote) {
            minNote = courant->coup.note;
        }

        for (int i = 0; i < courant->nbEnfants; i++) {
            enfiler(&tete, &queue, courant->enfants[i]);
        }
    }

    return pionMaxScore;
}
