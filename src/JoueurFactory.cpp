#include "JoueurFactory.h"
#include "Joueur.h"
#include <stdexcept>


std::unique_ptr<Joueur> FactoryJoueurHumain::creer(const JoueurConfig& cfg) const
{
    if (cfg.nom.empty())
        throw std::invalid_argument("Le nom du joueur humain ne peut pas être vide.");
    return std::unique_ptr<Joueur>(new Joueur(cfg.nom));
}


std::unique_ptr<Joueur> FactoryIllustreArchitecte::creer(const JoueurConfig& cfg) const
{
    int diff = cfg.difficulte;
    if (diff < 1 || diff > 3) diff = 1; 

    return std::unique_ptr<Joueur>(new IllustreArchitecte(diff));
}
