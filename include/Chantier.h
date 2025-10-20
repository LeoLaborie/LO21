#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include "Tuile.h"
#include "Joueur.h"

class Chantier
{
private:
    std::vector<Tuile> tuilesChantier;

public:
    Chantier() {};
    ~Chantier() = default;
    int taille() const { return tuilesChantier.size(); }
    void ajouterTuile(const Tuile &t) { tuilesChantier.push_back(t); }
    const std::vector<Tuile> &getTuiles() const { return tuilesChantier; }
    Tuile* piocherTuile(int id, Joueur joueur);
};

#endif
