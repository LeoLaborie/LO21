#ifndef CHANTIER_H
#define CHANTIER_H

#include "Tuile.h"

class Chantier {
    private:
        std::vector<Tuile> tuilesChantier;

public:
    Chantier(std::vector<Tuile> tuiles) : tuilesChantier(tuiles) {};
    ~Chantier();
    void ajouterTuile(const Tuile &t) { tuilesChantier.push_back(t); }
    const std::vector<Tuile> &getTuiles() const { return tuilesChantier; }
    bool piocherTuile(int id,Partie &partie){}
};

#endif