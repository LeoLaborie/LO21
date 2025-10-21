#ifndef JOUEUR_H
#define JOUEUR_H

#include <vector>
#include <iostream>
#include "Plateau.h"
#include "Tuile.h"
#include "Chantier.h"

class Joueur
{
private:
    int nbrPierres{};
    int nbrPoints{};
    Plateau plateau;
    Tuile tuileEnMain;

public:
    Joueur();
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