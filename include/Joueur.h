#ifndef JOUEUR_H
#define JOUEUR_H

#include <iostream>
#include "Plateau.h"

class Joueur
{
private:
    string nom;
    int nbrPierres;
    int nbrPoints;
    Plateau plateau;

public:
    void setNbrPierres(int);
    int getNbrPierres();
    void setNbrPoints(int);
    int getNbrPoints();

    Joueur(string n)
    {
        nom = n;
        nbrPierres = 0;
        nbrPoints = 0;
        plateau = Plateau();
    }
};

#endif