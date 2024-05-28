/**************************************************************************
 * Nom du fichier : file.h
 * Description    : Gestion d'une file pour le parcours d'un arbre avec plusieurs fils pour l'IA est l'algo MinMAx
 * Auteurs        : Team GEGK
 **************************************************************************/

#ifndef FILE_H
#define FILE_H

#include "arbre.h"

// Structure pour la file
typedef struct File {
    Noeud* noeud;
    struct File* suivant;
} File;

// Fonction pour ajouter un nœud à la file
void enfiler(File** tete, File** queue, Noeud* noeud);

// Fonction pour retirer un nœud de la file
Noeud* defiler(File** tete, File** queue);

#endif // FILE_H
