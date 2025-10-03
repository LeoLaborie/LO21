#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include "Tuile.h"
#include "Partie.h"
#include <math.h>

class Chantier
{
private:
    std::vector<Tuile> tuilesChantier;

public:
    Chantier(std::vector<Tuile> tuiles) : tuilesChantier(tuiles) {};
    ~Chantier() = default;
    int taille() const { return tuilesChantier.size(); }
    void ajouterTuile(const Tuile &t) { tuilesChantier.push_back(t); }
    const std::vector<Tuile> &getTuiles() const { return tuilesChantier; }
    bool piocherTuile(int id, Partie &partie);
};
std::vector<Tuile> genererPaquet(int taille, Partie &partie);
#endif
