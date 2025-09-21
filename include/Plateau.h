#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include "Tuile.h"
#include "Hexagone.h" 

class Plateau
{
private:
    std::vector<Tuile> listeTuiles;
    std::vector<Hexagone*> listeHexagones;

public:

    void ajouterTuile(const Tuile &t) { listeTuiles.push_back(t); }
    const std::vector<Tuile>& getTuiles() const { return listeTuiles; }

    void ajouterHexagone(Hexagone *h) { listeHexagones.push_back(h); }
    const std::vector<Hexagone*> getHexagones() const { return listeHexagones; }

    void updateVoisins();

    Plateau()
    {
        listeTuiles = {};
        listeHexagones = {};
    }


};

#endif
