#include "Plateau.h"
#include "iostream"

int main()
{
    Plateau p;

    for (const auto &hex : p.getHexagones())
    {
        std::cout << "Hexagone (" << hex->getX() << ", " << hex->getY() << ")"
                  << " a pour voisins : ";
        for (const auto &voisin : hex->getVoisins())
        {
            std::cout << "(" << voisin->getX() << ", " << voisin->getY() << ") ";
        }
        std::cout << std::endl;
    }

    return 0;
}
