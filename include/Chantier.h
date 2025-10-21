#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include "Tuile.h"

class Chantier
{
private:
    std::vector<Tuile> tuilesChantier;

public:
    Chantier() {};
    ~Chantier() = default;
    int getTaille() const { return tuilesChantier.size(); }
    void ajouterTuile(const Tuile &t) { tuilesChantier.push_back(t); }
    const std::vector<Tuile> &getTuiles() const { return tuilesChantier; }
    void retirerTuile(int id);
};

#endif
