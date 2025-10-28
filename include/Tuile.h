#ifndef TUILE_H
#define TUILE_H

#include <vector>
#include <iostream>
#include "Hexagone.h"

class Tuile
{
private:
    std::vector<Hexagone *> hex;

public:
    Tuile() = default;
    Tuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3);
    void creerTuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3);
    const std::vector<Hexagone *> &getHexagones() const { return hex; }

    void pivoterTuile()
    {
        Hexagone *tmp = hex[0];
        hex[0] = hex[1];
        hex[1] = hex[2];
        hex[2] = tmp;
    }

    void afficher() const {
        std::cout << "Tuile :\n";
        for (const auto& h : hex) {
            h->afficher();
        }
    }
};

#endif
