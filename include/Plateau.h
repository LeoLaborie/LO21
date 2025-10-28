#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "Tuile.h"
#include "Hexagone.h"
#include "Position.h"
#include "couleurs_console.h"

class Plateau
{
private:
    std::vector<Tuile> listeTuiles;
    std::vector<Hexagone *> listeHexagones;

public:
    explicit Plateau();

    bool verifierPlacementTuile(Position &p) const;
    int placerTuile(Tuile &t, Position &p);
    void updateVoisins();
    const std::vector<Tuile> &getTuiles() const { return listeTuiles; }
    const std::vector<Hexagone *> &getHexagones() const { return listeHexagones; }
    int calculerPoints() const;
    int calculerPointsHabitation() const;
    int calculerPointsMarche() const;
    int calculerPointsJardin() const;
    int calculerPointsTemple() const;
    int calculerPointsCaserne() const;
    void afficher() const {
        std::cout << "\nPlateau contient " << listeTuiles.size() << " tuiles :\n";
        std::cout << " ----\n";
        for (const auto& t : listeTuiles) {
            t.afficher();
            std::cout << " ----\n";
        }
    }
};

#endif
