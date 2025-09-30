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
    Joueur()
    {
        nbrPoints = 0;
        plateau = Plateau();
    }
    void setNbrPierres(int);
    int getNbrPierres();
    void setNbrPoints(int);
    int getNbrPoints();
};

#endif