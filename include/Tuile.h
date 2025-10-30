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
    Tuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3, Hexagone *hex4);
    void creerTuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3);
    const std::vector<Hexagone *> &getHexagones() const { return hex; }

    void pivoterTuile()
    {
        Hexagone *tmp = hex[0];
        hex[0] = hex[1];
        hex[1] = hex[2];
        hex[2] = tmp;
    }

    void afficher(bool court = false) const
    {
        if (court)
        {
            std::cout << "\t";
            hex[0]->afficher(true);
            std::cout << "\n";
            hex[1]->afficher(true);
            std::cout << " ";
            hex[2]->afficher(true);
            std::cout << "\n";
        }
        else
        {
            for (const auto &h : hex)
            {
                h->afficher();
            }
        }
    }
};

#endif
