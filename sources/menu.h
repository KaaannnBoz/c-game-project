#ifndef MENU_H
#define MENU_H

int ShowMenu(void);

#define NOMBRE_ITEMS_MENU    6

#define JEU_AUCUN -1
#define JEU_UN_JOUEUR 0
#define JEU_DEUX_JOUEURS 1
#define JEU_DEUX_IA 2
#define JEU_OPTIONS 3
#define JEU_A_PROPOS 4
#define JEU_QUITTER 5

static const char *menuText[] = {
    "UN JOUEUR",
    "DEUX JOUEURS",
    "DEUX IAS",
    "OPTIONS",
    "A PROPOS",
    "QUITTER"
};

#endif
