#include "Partie.h"

void Partie::setNbrJoueurs(int nbr)
{
    nbrJoueurs = nbr;
}

void Partie::tourTermine()
{
    if (maitreArchitecte < nbrJoueurs - 1)
        maitreArchitecte++;
    else
        maitreArchitecte = 0;

    nbrTours++;
}
