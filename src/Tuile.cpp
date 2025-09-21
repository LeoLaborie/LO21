#include "Tuile.h"

void Tuile::creerTuile(Hexagone *hex1, Hexagone *hex2, Hexagone *hex3)
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