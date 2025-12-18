#include "Chantier.h"

void Chantier::retirerTuile(int id)
{
    if (id >= 0 && static_cast<size_t>(id) < getTaille())
    {
        tuilesChantier.erase(tuilesChantier.begin() + id);
    }
}
