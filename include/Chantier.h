#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include "Tuile.h"


class Partie;

class Chantier {
private:
    std::vector<Tuile> tuilesChantier;

public:
    Chantier(std::vector<Tuile> tuiles);
    ~Chantier() = default; 

    void ajouterTuile(const Tuile& t) { tuilesChantier.push_back(t); }
    const std::vector<Tuile>& getTuiles() const { return tuilesChantier; }


    bool piocherTuile(int id, Partie& partie);
};

#endif 
