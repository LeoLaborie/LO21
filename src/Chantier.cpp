#include "Chantier.h"

void Chantier::insererTuile(int index, const Tuile& t)
{
    if (index < 0)
        index = 0;
    if (index > static_cast<int>(tuilesChantier.size()))
        index = static_cast<int>(tuilesChantier.size());
    tuilesChantier.insert(tuilesChantier.begin() + index, t);
}

int Chantier::indexOf(TuileId id) const
{
    for (size_t i = 0; i < tuilesChantier.size(); ++i)
    {
        if (tuilesChantier[i].getId() == id)
            return static_cast<int>(i);
    }
    return -1;
}

const Tuile* Chantier::trouverParId(TuileId id) const
{
    const int idx = indexOf(id);
    if (idx < 0)
        return nullptr;
    return &tuilesChantier[static_cast<size_t>(idx)];
}

Tuile* Chantier::trouverParId(TuileId id)
{
    const int idx = indexOf(id);
    if (idx < 0)
        return nullptr;
    return &tuilesChantier[static_cast<size_t>(idx)];
}

void Chantier::retirerTuile(TuileId id)
{
    const int idx = indexOf(id);
    if (idx >= 0)
        tuilesChantier.erase(tuilesChantier.begin() + idx);
}
