#include "StrategieScoreCaserne.h"

#include "Plateau.h"
#include "Hexagone.h"

int StrategieScoreCaserneBase::pointsPourUneCaserne_Base(const Hexagone* h)
{
    if (h->getEstRecouvert()) return 0;
    // <=5 voisins => (z+1) sinon 0
    return (h->getVoisins().size() <= 5) ? (h->getZ() + 1) : 0;
}

int StrategieScoreCaserneVariante::pointsPourUneCaserne_Variante(const Hexagone* h)
{
    int p = StrategieScoreCaserneBase::pointsPourUneCaserne_Base(h);
    if (p == 0) return 0;

    // variante : si <=3 voisins => double
    if (h->getVoisins().size() <= 3)
        p *= 2;

    return p;
}

int StrategieScoreCaserneBase::calculer(const Plateau& plateau) const
{
    int total = 0;
    int multiplicateur = 0;
    plateau.pourChaqueHexagone([&](const Hexagone* h){
        if (h->getEstRecouvert()) return;
        const TypeHex type = h->getType();
        if (type == TypeHex::Caserne)
        {
            total += pointsPourUneCaserne_Base(h);
        }
        else if (type == TypeHex::PCaserne)
        {
            multiplicateur += 2;
        }
    });
    return total * multiplicateur;
}

int StrategieScoreCaserneVariante::calculer(const Plateau& plateau) const
{
    int total = 0;
    int multiplicateur = 0;
    plateau.pourChaqueHexagone([&](const Hexagone* h){
        if (h->getEstRecouvert()) return;
        const TypeHex type = h->getType();
        if (type == TypeHex::Caserne)
        {
            total += pointsPourUneCaserne_Variante(h);
        }
        else if (type == TypeHex::PCaserne)
        {
            multiplicateur += 2;
        }
    });
    return total * multiplicateur;
}
