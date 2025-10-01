#include "Chantier.h"
#include "Partie.h"
#include <random>
#include <algorithm>
#include <iostream>


bool Chantier::piocherTuile(int id, Partie& partie) {
    Joueur& joueur = partie.getJoueurMain();

    if (id < 0 || id >= this->taille()) return false;
    if (id > joueur.getNbrPierres()) return false;
    joueur.setNbrPierres(joueur.getNbrPierres() - id);
    joueur.setTuileEnMain(tuilesChantier[id]);
    tuilesChantier.erase(tuilesChantier.begin() + id);
    if (this->taille() == 1) {
        partie.tourTermine();
        partie.setProchainJoueur();
        if (partie.getTaillePaquet() > 0) {
            std::vector<Tuile> nouveauPaquet = genererPaquet(partie.getTaillePaquet(), partie);
            for (const auto& tuile : nouveauPaquet) {
                this->ajouterTuile(tuile);
            }
        }
    }
    return true;
}

std::vector<Tuile> genererPaquet(int taille, Partie& partie) {
    std::vector<Tuile> paquet;
    std::vector<Tuile>& tuilesDisponibles = partie.getTuilesDisponibles();

    // on applique une fonction random pour simuler les paquets mélangés
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(tuilesDisponibles.begin(), tuilesDisponibles.end(), g);

    for (int i = 0; i < taille && i < (int)tuilesDisponibles.size(); ++i) {
        paquet.push_back(tuilesDisponibles[i]);
        tuilesDisponibles.erase(tuilesDisponibles.begin() + i);
    }

    return paquet;
}

