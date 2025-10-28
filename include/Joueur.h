#ifndef JOUEUR_H
#define JOUEUR_H

#include <vector>
#include <iostream>
#include "Plateau.h"
#include "Tuile.h"
#include "Chantier.h"

class Joueur
{
private:
    int nbrPierres{};
    int nbrPoints{};
    std::string nom = "";
    Plateau plateau;
    Tuile tuileEnMain;

public:
    Joueur();
    void setNom(const std::string &n) { nom = n; }
    const std::string &getNom() const { return nom; }
    void setNbrPierres(int);
    int getNbrPierres();
    void setNbrPoints();
    Plateau &getPlateau() { return plateau; }
    const Tuile &getTuileEnMain() const { return tuileEnMain; }
    int getNbrPoints();
    void setTuileEnMain(const Tuile &t) { tuileEnMain = t; }
    Tuile* piocherTuile(int id, Chantier& chantier);
    void afficher() const {
        std::cout << "Nom : " << nom << ", Pierres : " << nbrPierres << ", Points : " << nbrPoints << "\n";
        plateau.afficher();
    }
};

#endif
