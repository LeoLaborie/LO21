#ifndef JOUEUR_H
#define JOUEUR_H

#include <iostream>
#include "Plateau.h"

class Joueur
{
private:
    int nbrPierres;
    int nbrPoints;
    Plateau plateau;

public:
    void setNbrPierres(int);
    int getNbrPierres();
    void setNbrPoints(int);
    int getNbrPoints();
};

#endif