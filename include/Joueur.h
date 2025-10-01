#ifndef JOUEUR_H
#define JOUEUR_H

#include <iostream>
#include "Plateau.h"
#include "Tuile.h"

class Partie; 

class Joueur
{
private:
    int nbrPierres{};
    int nbrPoints{};
    Plateau plateau;
    Tuile tuileEnMain;

public:
    explicit Joueur(Partie& p);   
    void setNbrPierres(int);
    int getNbrPierres();
    void setNbrPoints(int);
    int getNbrPoints();
    void setTuileEnMain(const Tuile &t) { tuileEnMain = t; }
};

#endif // JOUEUR_H
