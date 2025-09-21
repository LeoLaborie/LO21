#include <Plateau.h>
#include <algorithm>

void Plateau::updateVoisins()
{
    // algo en O(n²), on pouurrait le rendre en O(n), mais vu qu'on a tres peu d'hexagone par plateau le n² n'est pas dérangeant 
    // Parcourir toutes les tuiles du plateau
    for (auto *&hexagone1 : listeHexagones)
    {
        for (auto *&hexagone2 : listeHexagones)
        {
            if (hexagone1 != hexagone2 &&
                std::abs(hexagone1->getX() - hexagone2->getX()) <= 1 &&
                std::abs(hexagone1->getY() - hexagone2->getY()) <= 1) 
            {
                // cherche si ils sont deja voisins ou non. Si ils ne le sont pas, -> addVoisin()
                if (std::find(hexagone1->getVoisins().begin(), hexagone1->getVoisins().end(), hexagone2) == hexagone1->getVoisins().end())
                {
                    hexagone1->addVoisin(hexagone2);
                }
                if (std::find(hexagone2->getVoisins().begin(), hexagone2->getVoisins().end(), hexagone1) == hexagone2->getVoisins().end())
                {
                    hexagone2->addVoisin(hexagone1);
                }
            }
        }
    }
}