#include "Chantier.h"

void Chantier::insererTuile(int index, const Tuile& t)
{
    if (index < 0)
        index = 0;
    if (index > static_cast<int>(tuilesChantier.size()))
        index = static_cast<int>(tuilesChantier.size());
    tuilesChantier.insert(tuilesChantier.begin() + index, t);
}

void Chantier::retirerTuile(int id)
{
    if (id >= 0 && static_cast<size_t>(id) < getTaille())
    {
        tuilesChantier.erase(tuilesChantier.begin() + id);
    }
}
