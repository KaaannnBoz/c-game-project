/**************************************************************************
 * Nom du fichier : arbre.h
 * Description    : Gestion d'un arbre avec plusieurs fils pour l'IA est l'algo MinMAx
 *                  L'arbre gérer est un arbre avec plusieurs fils
 *					Fonctions utiles de gestion d'arbre
 * Auteurs        : Team GEGK
 **************************************************************************/

#ifndef ARBRE_H
#define ARBRE_H
#include "player_ia.h"

// Définition de la structure Noeud
typedef struct Noeud {
    coupIA coup;
    struct Noeud** enfants;
    int nbEnfants;
    struct Noeud* parent; // Ajouter un pointeur vers le parent
} Noeud;

// Prototypes des fonctions

// Fonction pour créer un nouveau nœud
Noeud* creerNoeud(coupIA unCoup);

// Fonction pour ajouter un enfant à un nœud
void ajouterEnfant(Noeud* parent, Noeud* enfant);

// Fonction pour obtenir le parent d'un nœud
Noeud* obtenirParent(Noeud* noeud);

// Fonction pour libérer la mémoire allouée à l'arbre
void libererArbre(Noeud* racine);

// Fonction pour afficher l'arbre
void afficherArbre(Noeud* racine, int niveau);

// Fonction pour parcourir l'arbre en largeur
Noeud* parcoursLargeur(Noeud* racine);

#endif // ARBRE_H
