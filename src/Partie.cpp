#include "Partie.h"
#include <stdexcept>

void Partie::setNbrJoueurs(int nbr) {
    if (nbr <= 0) throw std::invalid_argument("nbrJoueurs doit etre > 0");
    nbrJoueurs = nbr;
    joueurs.assign(nbrJoueurs, Joueur{}); 
    mainJoueur = 0;                      
    maitreArchitecte = 0;                  
    nbrTours = 0;
}

void Partie::tourTermine() {
    if (nbrJoueurs > 0) {
        maitreArchitecte = (maitreArchitecte + 1) % nbrJoueurs;
    }
    nbrTours++;
}
