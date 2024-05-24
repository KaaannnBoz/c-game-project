#ifndef ARBRE_H
#define ARBRE_H

// Définition de la structure d'un nœud de l'arbre n-aire
typedef struct Noeud {
    int donnee;            // Donnée du nœud
    struct Noeud** enfants; // Tableau de pointeurs vers les enfants
    int nbEnfants;         // Nombre d'enfants
} Noeud;

// Fonction pour créer un nouveau nœud
Noeud* creerNoeud(int donnee);

// Fonction pour ajouter un enfant à un nœud
void ajouterEnfant(Noeud* parent, Noeud* enfant);

// Fonction pour libérer la mémoire allouée à l'arbre
void libererArbre(Noeud* racine);

// Fonction pour afficher l'arbre
void afficherArbre(Noeud* racine, int niveau);

// Fonction pour parcourir l'arbre en largeur
void parcoursLargeur(Noeud* racine);

#endif // ARBRE_H
