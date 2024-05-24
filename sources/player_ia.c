#include "player_ia.h"
#include <stdlib.h>
#include <time.h>
#include "arbre.h"
#include "file.h"
#include "game.h"

int main2() {
    // Création des nœuds de l'arbre
    Noeud* racine = creerNoeud(1);
    Noeud* enfant1 = creerNoeud(2);
    Noeud* enfant2 = creerNoeud(3);
    Noeud* enfant3 = creerNoeud(4);

    // Ajout des enfants au nœud racine
    ajouterEnfant(racine, enfant1);
    ajouterEnfant(racine, enfant2);
    ajouterEnfant(racine, enfant3);

    // Ajout d'enfants à l'un des nœuds enfants
    ajouterEnfant(enfant1, creerNoeud(5));
    ajouterEnfant(enfant1, creerNoeud(6));

    // Affichage de l'arbre
    afficherArbre(racine, 0);

    // Parcours en largeur de l'arbre
    printf("Parcours en largeur : ");
    parcoursLargeur(racine);

    // Libération de la mémoire allouée à l'arbre
    libererArbre(racine);

    return 0;
}

void SimulatePlayer(pionGrille *pion) {
    // Simuler le déplacement automatique du joueur (contrôlé par l'ordinateur)

    // Générer un mouvement aléatoire pour le joueur
    int randomDirection = rand() % 4; // Génère un nombre aléatoire entre 0 et 3

    switch (randomDirection) {
        case 0: // Déplacement vers la droite
            deplacerPion(pion,pion->positionLigne, pion->positionColonne + 1);
            break;
        case 1: // Déplacement vers la gauche
            deplacerPion(pion,pion->positionLigne, pion->positionColonne - 1);
            break;
        case 2: // Déplacement vers le bas
            deplacerPion(pion,pion->positionLigne+1, pion->positionColonne);
            break;
        case 3: // Déplacement vers le haut
            deplacerPion(pion,pion->positionLigne-1, pion->positionColonne);
            break;
        default:
            break;
    }
}
