#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "Tuile.h"
#include "Hexagone.h"
#include "Position.h"
#include "utils.h"

class Plateau
{
private:
    std::vector<Tuile> listeTuiles;
    std::vector<Hexagone *> listeHexagones;

public:
    explicit Plateau();

    bool verifierPlacementTuile(Position &p) const;
    Position *essayerPlacerTuile(Tuile &t);
    bool ajouterTuile(Tuile &t, Position &p);
    void updateVoisins();
    const std::vector<Tuile> &getTuiles() const { return listeTuiles; }
    const std::vector<Hexagone *> &getHexagones() const { return listeHexagones; }
    int calculerPoints() const;
    void afficher() const;
};

#endif
