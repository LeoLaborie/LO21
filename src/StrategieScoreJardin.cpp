#include "StrategieScoreJardin.h"

#include "Plateau.h"
#include "Hexagone.h"

#include <array>
#include <utility>

int StrategieScoreJardinBase::pointsPourUnJardin_Base(const Hexagone* h)
{
    if (h->getEstRecouvert()) return 0;
    return (h->getZ() + 1);
}

int StrategieScoreJardinBase::calculer(const Plateau& plateau) const
{
    int total = 0;
    int multiplicateur = 0;
    plateau.pourChaqueHexagone([&](const Hexagone* h){
        if (h->getEstRecouvert()) return;
        const TypeHex type = h->getType();
        if (type == TypeHex::Jardin)
            total += pointsPourUnJardin_Base(h);
        else if (type == TypeHex::PJardin)
            multiplicateur += 3;
    });
    return total * multiplicateur;
}

bool StrategieScoreJardinVariante::estLac(const Plateau& plateau, const Hexagone* jardin)
{
    static const std::array<std::pair<int,int>,6> dirs = {{
        {+1,0}, {+1,-1}, {0,-1}, {-1,0}, {-1,+1}, {0,+1}
    }};

    const int x0 = jardin->getX();
    const int y0 = jardin->getY();
    const int z0 = jardin->getZ();

    for (auto [dx,dy] : dirs)
    {
        bool found = false;
        plateau.pourChaqueHexagone([&](const Hexagone* h){
            if (found) return;
            if (h->getEstRecouvert()) return;
            if (h->getX() == x0 + dx && h->getY() == y0 + dy && h->getZ() == z0)
                found = true;
        });
        if (!found) return false;
    }
    return true;
}

int StrategieScoreJardinVariante::calculer(const Plateau& plateau) const
{
    int total = 0;
    int multiplicateur = 0;

    plateau.pourChaqueHexagone([&](const Hexagone* h){
        if (h->getEstRecouvert()) return;
        const TypeHex type = h->getType();
        if (type == TypeHex::Jardin)
        {
            int p = pointsPourUnJardin_Base(h);
            if (estLac(plateau, h))
                p += (h->getZ() + 1);
            total += p;
        }
        else if (type == TypeHex::PJardin)
        {
            multiplicateur += 3;
        }
    });

    return total * multiplicateur;
}
