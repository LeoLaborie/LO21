#include "Tuile.h"

Tuile::Tuile(Hexagone* hex1, Hexagone* hex2, Hexagone* hex3, Hexagone* hex4, TuileId forcedId)
    : id(forcedId != 0 ? forcedId : nextId++)
{
    const TuileId uid = getId();
    hex1->setParent(uid);
    hex2->setParent(uid);
    hex3->setParent(uid);
    hex4->setParent(uid);
    // z = niveau (hauteur) : une tuile "en main" est toujours au niveau 0.
    // Les positions relatives de la tuile sont portées par `offsets` (q,r).
    hex1->setCoord(0, 0, 0);
    hex2->setCoord(0, -1, 0);
    hex3->setCoord(-1, 1, 0);
    hex4->setCoord(1, 0, 0);
    hex1->setVoisins({hex2, hex3, hex4});
    hex2->setVoisins({hex1, hex3, hex4});
    hex3->setVoisins({hex1, hex2, hex4});
    hex4->setVoisins({hex1, hex2, hex3});
    hex.clear();
    hex.reserve(4);
    hex.emplace_back(hex1);
    hex.emplace_back(hex2);
    hex.emplace_back(hex3);
    hex.emplace_back(hex4);

    offsets.clear();
    offsets.reserve(4);

    offsets.push_back({0, 0});   // le milieu
    offsets.push_back({0, -1});  // bas-gauche
    offsets.push_back({-1, 1});  // bas-centre
    offsets.push_back({1, 0});   // bas-droit

    if (forcedId != 0 && forcedId >= nextId)
        nextId = forcedId + 1;
}

Tuile::Tuile(Hexagone* hex1, Hexagone* hex2, Hexagone* hex3, TuileId forcedId)
    : id(forcedId != 0 ? forcedId : nextId++)
{
    const TuileId uid = getId();
    hex1->setParent(uid);
    hex2->setParent(uid);
    hex3->setParent(uid);
    hex1->setCoord(0, 0, 0);
    // z = niveau (hauteur) : une tuile "en main" est toujours au niveau 0.
    hex2->setCoord(1, -1, 0);
    hex3->setCoord(0, -1, 0);
    hex1->setVoisins({hex2, hex3});
    hex2->setVoisins({hex1, hex3});
    hex3->setVoisins({hex1, hex2});

    hex.clear();
    hex.reserve(3);
    hex.emplace_back(hex1);
    hex.emplace_back(hex2);
    hex.emplace_back(hex3);

    offsets.clear();
    offsets.reserve(3);
    offsets.push_back({0, 0});
    offsets.push_back({+1, -1});
    offsets.push_back({0, -1});

    if (forcedId != 0 && forcedId >= nextId)
        nextId = forcedId + 1;
}

TuileId Tuile::getNextId()
{
    return nextId;
}

void Tuile::setNextId(TuileId next)
{
    if (next == 0)
        next = 1;
    nextId = next;
}
