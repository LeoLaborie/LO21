#include "Partie.h"

void Partie::genererTuilesParties(){}

void Partie::setNbrJoueurs(int nbr)
{
    int nbpierre = 1;
    if (nbr <= 0 || nbr > 4)
        throw std::invalid_argument("nbrJoueurs doit etre > 0 et inférieur à 4");
    nbrJoueurs = nbr;
    joueurs.assign(nbrJoueurs, Joueur{});
    for (auto &j : joueurs)
    {
        j.setNbrPierres(nbpierre);
        nbpierre++;
    }
    mainJoueur = 0;
    maitreArchitecte = 0;
    nbrTours = 0;
    taillepaquet = 1 + nbrJoueurs;
}

void Partie::tourTermine()
{
    if (nbrJoueurs > 0)
    {
        maitreArchitecte = (maitreArchitecte + 1) % nbrJoueurs;
    }
    nbrTours++;
}

std::vector<Tuile> Partie::retirerPaquet(int taille)
{
    std::vector<Tuile> paquet;

    // on applique une fonction random pour simuler les paquets mélangés
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(tuilesPartie.begin(), tuilesPartie.end(), g);

    for (int i = 0; i < taille && i < (int)tuilesPartie.size(); ++i)
    {
        paquet.push_back(tuilesPartie[i]);
        tuilesPartie.erase(tuilesPartie.begin() + i);
    }

    return paquet;
}