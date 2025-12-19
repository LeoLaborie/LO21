#include "StrategieScoreMarche.h"

#include "Plateau.h"
#include "Hexagone.h"

int StrategieScoreMarcheBase::pointsPourUnMarche_Base(const Hexagone* h)
{
    if (h->getEstRecouvert()) return 0;

    // si voisin marche => 0
    for (const auto& v : h->getVoisins())
        if (v->getType() == TypeHex::Marche)
            return 0;

    return h->getZ() + 1;
}

int StrategieScoreMarcheVariante::pointsPourUnMarche_Variante(const Hexagone* h)
{
    int p = StrategieScoreMarcheBase::pointsPourUnMarche_Base(h);
    if (p == 0) return 0;

    // variante : bonus si voisin place marché (PMarche)
    for (const auto& v : h->getVoisins())
    {
        if (v->getType() == TypeHex::PMarche)
        {
            p += h->getZ(); // exactement comme ton code : nbpoint += h->getZ();
            break;
        }
    }
    return p;
}

int StrategieScoreMarcheBase::calculer(const Plateau& plateau) const
{
    int total = 0;
    int multiplicateur = 0;
    plateau.pourChaqueHexagone([&](const Hexagone* h){
        if (h->getEstRecouvert()) return;
        const TypeHex type = h->getType();
        if (type == TypeHex::Marche)
        {
            total += pointsPourUnMarche_Base(h);
        }
        else if (type == TypeHex::PMarche)
        {
            multiplicateur += 2;
        }
    });
    return total * multiplicateur;
}

int StrategieScoreMarcheVariante::calculer(const Plateau& plateau) const
{
    int total = 0;
    int multiplicateur = 0;
    plateau.pourChaqueHexagone([&](const Hexagone* h){
        if (h->getEstRecouvert()) return;
        const TypeHex type = h->getType();
        if (type == TypeHex::Marche)
        {
            total += pointsPourUnMarche_Variante(h);
        }
        else if (type == TypeHex::PMarche)
        {
            multiplicateur += 2;
        }
    });
    return total * multiplicateur;
}
