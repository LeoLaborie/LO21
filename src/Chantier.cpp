#include "Chantier.h"

Tuile* Chantier::piocherTuile(int id, Joueur joueur)
{
    if (id < 0 || id >= this->taille())
        return nullptr;
    if (id > joueur.getNbrPierres())
        return nullptr;
    joueur.setNbrPierres(joueur.getNbrPierres() - id);
    joueur.setTuileEnMain(tuilesChantier[id]);
    tuilesChantier.erase(tuilesChantier.begin() + id);
    return &tuilesChantier[id];
}
