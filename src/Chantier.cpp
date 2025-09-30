#include "Chantier.h"
#include "Partie.h"



bool Chantier::piocherTuile(int id, Partie& partie) {
    Joueur& joueur = partie.getJoueurMain();

    if (id > joueur.getNbrPierres()) {
        return false;
    }
    return true;
}
