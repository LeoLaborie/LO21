#ifndef TUILE_H
#define TUILE_H

#include <iostream>
#include <vector>
#include "Hexagone.h"

class Tuile
{
private:
    std::vector<Hexagone *> hex;

public:
    void creerTuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3);

    const std::vector<Hexagone*>& getHexagones() const { return hex; } // retourne les hexagones d'une tuile
    
    void pivoterTuile()    //permet de pivoter une tuile dans le sens horaire
    {
        Hexagone* tmp = hex[0];
        hex[0] = hex[1];
        hex[1] = hex[2];
        hex[2] = tmp;
    }
    Tuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3)
    {
        hex1->setParent(this); // définir le parent de chaque hexagone comme cette tuile
        hex2->setParent(this);
        hex3->setParent(this);

        hex1->setVoisins({hex2, hex3}); // définir les voisins de chaque hexagone
        hex2->setVoisins({hex1, hex3});
        hex3->setVoisins({hex1, hex2});

        hex.push_back(hex1); // on ajoute la liste des héxagones à la tuile
        hex.push_back(hex2);
        hex.push_back(hex3);
    }
};

#endif
