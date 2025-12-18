#include "JoueurFactory.h"
#include "Joueur.h"
#include <stdexcept>


std::unique_ptr<Joueur> FactoryJoueurHumain::creer(const JoueurConfig& cfg) const
{
    const std::string nomFinal = cfg.nom.empty() ? "joueur" : cfg.nom;
    static const bool variantesDefaut[5] = {false, false, false, false, false};
    const bool* variantes = cfg.variantesScore ? cfg.variantesScore : variantesDefaut;
    return std::unique_ptr<Joueur>(new Joueur(variantes, nomFinal));
}


std::unique_ptr<Joueur> FactoryIllustreArchitecte::creer(const JoueurConfig& cfg) const
{
    int diff = cfg.difficulte;
    if (diff < 1 || diff > 3) diff = 1; 

    return std::unique_ptr<Joueur>(new IllustreArchitecte(diff));
}