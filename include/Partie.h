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
    int maitreArchitecte; // indice du joueur qui est maitre architecte
    int nbrTours;
    int taillepaquet; //nombre de tuiles par paquet
    int nbpaquets;    //on a pas besion de stocker les paquets, on fera juste une liste de tuiles et on en tirera au hasard au sort le nombre de tuiles necessaires
    vector<Tuile> tuilesPartie;   
    vector<Joueur> joueurs;
    int mainJoueur; // indice du joueur dont c'est le tour

public:
    Partie()=default;
    void setNbrJoueurs(int nbr);
    int getNbrJoueurs() const { return nbrJoueurs; }
    void tourTermine();
    Joueur& getJoueurMain() { return joueurs[mainJoueur]; }
    void setProchainJoueur() { mainJoueur = (mainJoueur + 1) % nbrJoueurs; }
    int getTaillePaquet() const { return taillepaquet; }
    vector<Tuile>& getTuilesDisponibles() { return tuilesPartie; }
    void setMaitreArchitecte(int i) { maitreArchitecte = i; }
    int getMaitreArchitecte() const { return maitreArchitecte; }
};

#endif


