#ifndef CHANTIER_H
#define CHANTIER_H

#include <vector>
#include <iostream>
#include "Tuile.h"
#include "couleurs_console.h"

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

    friend std::ostream& operator<<(std::ostream& os,const Chantier& c){
        os << "\nChantier contient " << c.tuilesChantier.size() << " tuiles :\n";
        for (size_t i = 0; i < c.tuilesChantier.size(); i++) {
            texte_gras_on();
            os << "\n\t ---- " << i << " ----\n\n";
            texte_reset();
            os<<c.tuilesChantier[i];
        }
        os << std::endl;
        return os;
    }
};

#endif
