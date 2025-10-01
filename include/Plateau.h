#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include "Tuile.h"
#include "Hexagone.h"

class Joueur;

class Plateau {
private:
    std::vector<Tuile> listeTuiles;
    std::vector<Hexagone*> listeHexagones;
    Joueur * proprietaire;

public:
    explicit Plateau(Joueur* j);

    void ajouterTuile(Tuile& t, int x, int y, int z);
    bool verifierPlacementTuile(int x, int y, int z) const;
    void updateVoisins();
    const std::vector<Tuile>&  getTuiles() const { return listeTuiles; }
    const std::vector<Hexagone*>&  getHexagones() const { return listeHexagones; }
    int calculerPoints() const;
};

#endif
