#include "StrategieScoreTemple.h"

#include "Plateau.h"
#include "Hexagone.h"

int StrategieScoreTempleBase::pointsPourUnTemple_Base(const Hexagone* h)
{
    if (h->getEstRecouvert()) return 0;
    return (h->getVoisins().size() == 6) ? (h->getZ() + 1) : 0;
}

int StrategieScoreTempleVariante::pointsPourUnTemple_Variante(const Hexagone* h)
{
    int p = StrategieScoreTempleBase::pointsPourUnTemple_Base(h);
    if (p == 0) return 0;
    if (h->getZ() >= 1)
        p *= 2;

    return p;
}

int StrategieScoreTempleBase::calculer(const Plateau& plateau) const
{
    int total = 0;
    int multiplicateur = 0;
    plateau.pourChaqueHexagone([&](const Hexagone* h){
        if (h->getEstRecouvert()) return;
        const TypeHex type = h->getType();
        if (type == TypeHex::Temple)
        {
            total += pointsPourUnTemple_Base(h);
        }
        else if (type == TypeHex::PTemple)
        {
            multiplicateur += 2;
        }
    });
    return total * multiplicateur;
}

int StrategieScoreTempleVariante::calculer(const Plateau& plateau) const
{
    int total = 0;
    int multiplicateur = 0;
    plateau.pourChaqueHexagone([&](const Hexagone* h){
        if (h->getEstRecouvert()) return;
        const TypeHex type = h->getType();
        if (type == TypeHex::Temple)
        {
            total += pointsPourUnTemple_Variante(h);
        }
        else if (type == TypeHex::PTemple)
        {
            multiplicateur += 2;
        }
    });
    return total * multiplicateur;
}
