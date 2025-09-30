#ifndef PARTIE_H
#define PARTIE_H

#include <iostream>
#include <string>
#include <vector>
#include "Tuile.h"
#include "Joueur.h"  

using namespace std;

class Partie
{
private:
    int nbrJoueurs;
    int maitreArchitecte;
    int nbrTours;
    vector<Tuile> chantier;
    vector<vector<Tuile>> paquets;
    vector<Joueur> joueurs;
    int mainJoueur; // indice du joueur dont c'est le tour

public:
    void setNbrJoueurs(int nbr);
    void tourTermine();
    Joueur& getJoueurMain() { return joueurs[mainJoueur]; }
};

#endif


