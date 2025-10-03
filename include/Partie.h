#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include "Tuile.h"
#include "Joueur.h"

class Partie
{
private:
    int nbrJoueurs = 0;
    int maitreArchitecte = 0; // indice du joueur maître architecte
    int mainJoueur = 0;       // indice du joueur dont c’est le tour
    int nbrTours = 0;
    int taillepaquet = 0;

    std::vector<Joueur> joueurs;
    std::vector<Tuile> tuilesPartie;

public:
    Partie() = default;

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

    int getTaillePaquet() const { return taillepaquet; }
    void setTaillePaquet(int t) { taillepaquet = t; }
    std::vector<Tuile> &getTuilesDisponibles() { return tuilesPartie; }

    int getNbrTours() const { return nbrTours; }
};

#endif
