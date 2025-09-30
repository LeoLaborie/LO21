#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include "Tuile.h"
#include "Hexagone.h"

class Plateau
{
private:
    std::vector<Tuile> listeTuiles;
    std::vector<Hexagone *> listeHexagones;

public:
    void ajouterTuile(const Tuile &t) { listeTuiles.push_back(t); }
    const std::vector<Tuile> &getTuiles() const { return listeTuiles; }

    void ajouterHexagone(Hexagone *h) { listeHexagones.push_back(h); }
    const std::vector<Hexagone *> getHexagones() const { return listeHexagones; }
    bool verifierPlacementTuile(const Tuile &t) const;
    void updateVoisins();

    Plateau()
    {
        listeTuiles = {};
        listeHexagones = 
        {
            // La tuile de départ contient 4 hexagone
            new Place(0, 0, 0, TypePlace::Habitation , 1),
            new Carriere(-1, 1, 0),
            new Carriere(0, -1, 0),
            new Carriere(1, 0, 0)
        };
        
        updateVoisins();

    }
};

#endif
