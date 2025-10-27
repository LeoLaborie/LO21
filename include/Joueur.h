#ifndef JOUEUR_H
#define JOUEUR_H

#include <vector>
#include <iostream>
#include "Plateau.h"
#include "Tuile.h"
#include "Chantier.h"
#include "Partie.h"

class Joueur
{
friend class Partie;
private:
    int nbrPierres{};
    int nbrPoints{};
    Plateau plateau;
    Tuile tuileEnMain;
    Joueur();// le joueur est créé au début de la partie, par la partie (composition)

public:
    void setNbrPierres(int);
    int getNbrPierres();
    void setNbrPoints();
    Plateau &getPlateau() { return plateau; }
    const Tuile &getTuileEnMain() const { return tuileEnMain; }
    int getNbrPoints();
    void setTuileEnMain(const Tuile &t) { tuileEnMain = t; }
    Tuile* piocherTuile(int id, Chantier& chantier);
};

#endif