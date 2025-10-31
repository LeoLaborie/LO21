#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include <math.h>
#include <stdexcept>
#include <random>
#include <map>
#include <string>
#include "Tuile.h"
#include "Joueur.h"
#include "Chantier.h"

class Partie
{
private:
    int nbrJoueurs = 0;
    int maitreArchitecte = 0; // indice du joueur maître architecte
    int mainJoueur = 0;       // indice du joueur dont c’est le tour
    int nbrTours = 0;
    int taillepaquet = 0;

    Chantier chantier;
    std::vector<Joueur> joueurs;
    std::vector<std::vector<Tuile>> piles;


public:
    Partie() = default; // il faut gérer les variantes de taille de paquet dans le constructeur

    void addTuileInChantierFromPiles();

    // Configuration des joueurs
    void setNbrJoueurs(int nbr);
    int getNbrJoueurs() const { return nbrJoueurs; }

    // Tour / maître architecte
    void tourTermine();
    void setMaitreArchitecte(int i) { maitreArchitecte = i % (nbrJoueurs > 0 ? nbrJoueurs : 1); }
    int getMaitreArchitecte() const { return maitreArchitecte; }

    // Navigation des joueurs
    Joueur &getJoueurMain() { return joueurs.at(mainJoueur); }
    const Joueur &getJoueurMain() const { return joueurs.at(mainJoueur); }
    void setProchainJoueur()
    {
        if (nbrJoueurs)
            mainJoueur = (mainJoueur + 1) % nbrJoueurs;
    }

    int getMainJoueur() const { return mainJoueur; }

    int getTaillePaquet() const { return taillepaquet; }
    void setTaillePaquet(int t) { taillepaquet = t; }
    Chantier& getChantier() { return chantier; }

    int getNbrTours() const { return nbrTours; }
    std::vector<Tuile> retirerPaquet();

    bool pilesRestantes() const { return !piles.empty(); }
    int getNbrPiles() const { return piles.size(); }

    void genererTuilesParties(bool fullTuiles = false);
};

#endif
