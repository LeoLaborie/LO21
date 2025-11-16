#include "Tuile.h"

Tuile::Tuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3, Hexagone *hex4){
    hex1->setParent(this);
    hex2->setParent(this);
    hex3->setParent(this);
    hex4->setParent(this);

    hex1->setVoisins({hex2, hex3, hex4});
    hex2->setVoisins({hex1, hex3, hex4});
    hex3->setVoisins({hex1, hex2, hex4});
    hex4->setVoisins({hex1, hex2, hex3});

    hex.push_back(hex1);
    hex.push_back(hex2);
    hex.push_back(hex3);
    hex.push_back(hex4);
}

Tuile::Tuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3)
{
    hex1->setParent(this);
    hex2->setParent(this);
    hex3->setParent(this);

    hex1->setVoisins({hex2, hex3});
    hex2->setVoisins({hex1, hex3});
    hex3->setVoisins({hex1, hex2});

    hex.push_back(hex1);
    hex.push_back(hex2);
    hex.push_back(hex3);
}

void Tuile::creerTuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3)
{
    hex1->setParent(this);
    hex2->setParent(this);
    hex3->setParent(this);

    hex1->setVoisins({hex2, hex3});
    hex2->setVoisins({hex1, hex3});
    hex3->setVoisins({hex1, hex2});

    hex.push_back(hex1);
    hex.push_back(hex2);
    hex.push_back(hex3);
}