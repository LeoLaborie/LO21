#include "Joueur.h"

int Joueur::getNbrPierres(){
    return nbrPierres;
}

int Joueur::getNbrPoints(){
    return nbrPoints;
}

void Joueur::setNbrPierres(int nbr){
    nbrPierres = nbr;
}

void Joueur::setNbrPoints(int nbr){
    nbrPoints = nbr;
}