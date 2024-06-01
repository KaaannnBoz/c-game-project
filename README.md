# c-game-project

L’objet de ce projet est de coder un jeu mettant en scène un affrontement stratégique entre deux groupes de combattants, dans lequel le joueur pourra contrôler un des camps contre un autre joueur ou une IA

## CONSIGNES – VALIDATION PAS PAR PAS:

### Compilation sans erreur
une seule erreur dans la bibliothèque Raylib. 
Aucune erreur dans notre code
### Pas de warnings
Aucun
### Le code a été commenté partout
Dans chaque fichier et chaque fonction
### Partie 1 & 2
Sélection et déplacement des unités possibles :
    Respecte les caractéristiques des pions.
    Les pions fatigués ne peuvent pas bouger.
Les 4 types d’unités ont été implémentées (voir fichier game.h pour structure pions et initialisations.c )
### Partie 3
	Affichage du jeu en mode « damier » avec possibilité de changer taille grille
	Support du clavier et/ou souris (voir par exemple menu.c)
	Absence de bugs graphiques : pas de bugs – astuce pour afficher quand IA va trop vite (voir game.c[65] clock)
### Partie 4
	Possibilité de jouer contre une IA, mode un joueur (voir arbres.c et player_ia.c)
	Prévision sur 3 coups : Utilisation de l’algorithme minimax alpha beta
### Partie 4b
	Affrontement entre deux IA : L’affichage de l’arbre des coups ralenti l’IA
	Algorithme génétique de perfectionnement de l’IA : Non fait
![img.png](img.png)