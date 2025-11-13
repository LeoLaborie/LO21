#include "Tuile.h"

Tuile::Tuile(Hexagone* hex1, Hexagone* hex2, Hexagone* hex3, Hexagone* hex4) {
    
    hex1->setParent(this); hex2->setParent(this); hex3->setParent(this); hex4->setParent(this);
    hex1->setVoisins({hex2, hex3, hex4});
    hex2->setVoisins({hex1, hex3, hex4});
    hex3->setVoisins({hex1, hex2, hex4});
    hex4->setVoisins({hex1, hex2, hex3});
    hex = {hex1, hex2, hex3, hex4};
    
    offsets.clear(); 
    offsets.reserve(4);
    
    offsets.push_back({ 0, 0});   // le milieu
    offsets.push_back({0, -1});   // bas-gauche
    offsets.push_back({ -1, 1});   // bas-centre
    offsets.push_back({ 1, 0});   // bas-droit

}

Tuile::Tuile(Hexagone* hex1, Hexagone* hex2, Hexagone* hex3) {
    hex1->setParent(this); hex2->setParent(this); hex3->setParent(this);
    hex1->setVoisins({hex2, hex3});
    hex2->setVoisins({hex1, hex3});
    hex3->setVoisins({hex1, hex2});
    
    hex = {hex1, hex2, hex3};
    
    offsets.clear(); offsets.reserve(3);
    offsets.push_back({ 0, 0});
    offsets.push_back({+1, -1});
    offsets.push_back({ 0, -1});
}

