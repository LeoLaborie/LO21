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
    vector<vector<Tuile>> piles;
    vector<Joueur> joueurs;

public:
    void setNbrJoueurs(int nbr);
    int getNbrJoueurs() { return nbrJoueurs; }

    void setMaitreArchitecte(int m) { maitreArchitecte = m; }
    int getMaitreArchitecte() { return maitreArchitecte; }

    void setNbrTours(int n) { nbrTours = n; }
    int getNbrTours() { return nbrTours; }

    void addTuileInChantierFromPiles();

    void tourTermine();

    void addJoueur(Joueur j);
    vector<Joueur> &getJoueurs() { return joueurs; }

    const vector<Tuile> &getChantier() const { return chantier; }

    void preparerPartie();

    Partie(int nJ)
    {
        nbrJoueurs = nJ;
        maitreArchitecte = 0;
        nbrTours = 0;
        chantier = {};
        piles = {};
        joueurs = {};
    }
};

#endif
