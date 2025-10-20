#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "Tuile.h"
#include "Hexagone.h"

struct C
{
    int x, y, z;
};

class Plateau
{
private:
    std::vector<Tuile> listeTuiles;
    std::vector<Hexagone *> listeHexagones;

public:
    explicit Plateau();

    bool verifierPlacementTuile(C& c) const;
    C* essayerPlacerTuile(Tuile &t);
    bool ajouterTuile(Tuile &t, C& c);
    void updateVoisins();
    const std::vector<Tuile> &getTuiles() const { return listeTuiles; }
    const std::vector<Hexagone *> &getHexagones() const { return listeHexagones; }
    int calculerPoints() const;
    void afficher() const;
    static std::vector<C> grillePetite(int r = 3);
};

#endif
