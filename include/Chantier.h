#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include <iostream>
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

    void afficher() const {
        std::cout << "\nChantier contient " << tuilesChantier.size() << " tuiles :\n";
        for (size_t i = 0; i < tuilesChantier.size(); i++) {
            std::cout << "\n --- " << i << " ---\n";
            tuilesChantier[i].afficher();
        }
        std::cout << std::endl;
    }
};

#endif
