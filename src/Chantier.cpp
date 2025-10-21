#include "Chantier.h"

void Chantier::retirerTuile(int id)
{
    if (id >= 0 && id < getTaille())
    {
        tuilesChantier.erase(tuilesChantier.begin() + id);
    }
}
