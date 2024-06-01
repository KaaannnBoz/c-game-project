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
    coupIA coup;            // Coup  jouer
    struct Noeud** enfants; // Liste des tous les noeud enfants : coups possibles
    int nbEnfants;          // Nombre de noeuds enfants
    struct Noeud* parent;   // Pointeur sur le noeud parent
    int note;               // Note sur le noeud
    bool est_terminal;      // Indique si le noeud est une feuille
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

// Fonction pour afficher un noeud
void afficherNoeud(Noeud* noeud);

// Fonction pour parcourir l'arbre en largeur MiniMax
Noeud* lancerMinimax(Noeud* racine);

#endif // ARBRE_H
