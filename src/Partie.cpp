#include "Partie.h"
#include <stdexcept>

void Partie::setNbrJoueurs(int nbr) {
    int nbpierre=1;
    if (nbr <= 0) throw std::invalid_argument("nbrJoueurs doit etre > 0");
    nbrJoueurs = nbr;
    joueurs.assign(nbrJoueurs, Joueur{}); 
    for (auto& j : joueurs) {
        j.setNbrPierres(nbpierre); 
        nbpierre++;  
    }
    mainJoueur = 0;                      
    maitreArchitecte = 0;                  
    nbrTours = 0;
    this->setTaillePaquet(2+nbrJoueurs);
}

void Partie::tourTermine() {
    if (nbrJoueurs > 0) {
        maitreArchitecte = (maitreArchitecte + 1) % nbrJoueurs;
    }
    nbrTours++;
}
