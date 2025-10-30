#ifndef PLATEAU_H
#define PLATEAU_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "Tuile.h"
#include "Hexagone.h"
#include "Position.h"
#include "couleurs_console.h"

// Déclarations anticipées pour l'amitié
class Partie;
class Joueur;

class Plateau
{
    friend class Partie;
    friend class Joueur;

private:
    std::vector<Tuile> listeTuiles;
    std::vector<Hexagone *> listeHexagones;

    bool varianteHabitation{0};
    bool varianteMarche{0};
    bool varianteTemple{0};
    bool varianteCaserne{0};
    bool varianteJardin{0};

    explicit Plateau(); // Les plateaux sont définis par le constructeur de Joueur

public:
    bool verifierPlacementTuile(Position &p, Tuile &t) const;
    int placerTuile(Tuile &t, Position &p);
    void updateVoisins();
    const std::vector<Tuile> &getTuiles() const { return listeTuiles; }
    const std::vector<Hexagone *> &getHexagones() const { return listeHexagones; }
    int calculerPoints() const;
    int calculerPointsHabitation() const;
    int calculerPointsMarche() const;
    int calculerPointsJardin() const;
    bool conditionVarianteJardin(const Hexagone* q) const;
    int calculerPointsTemple() const;
    int calculerPointsCaserne() const;
    void afficher() const
    {
        std::cout << "\nPlateau contient " << listeTuiles.size() << " tuiles :\n";
        std::cout << " ----\n";
        for (const auto &t : listeTuiles)
        {
            t.afficher();
            std::cout << " ----\n";
        }
    }
};

#endif
