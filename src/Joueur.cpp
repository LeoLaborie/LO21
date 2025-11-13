#include "Joueur.h"

Joueur::Joueur(bool varianteScore,std::string nom): nbrPierres(0),nbrPoints(0),nom(nom),plateau(varianteScore) {};

int Joueur::getNbrPierres()
{
    return nbrPierres;
}

int Joueur::getNbrPoints()
{
    return nbrPoints;
}

void Joueur::setNbrPierres(int nbr)
{
    nbrPierres = nbr;
}

void Joueur::setNbrPoints()
{
    nbrPoints = getPlateau().calculerPoints();
}

Tuile* Joueur::piocherTuile(int id, Chantier& chantier)
{
    if (id < 0 || id >= chantier.getTaille())
        return nullptr;
    if (id > getNbrPierres())
        return nullptr;
    setNbrPierres(getNbrPierres() - id);
    setTuileEnMain(chantier.getTuiles()[id]);
    chantier.retirerTuile(id);
    return &tuileEnMain;
}

bool Joueur::placerTuile(Tuile &t, Position &p)
{
    int carrieresCouvertes = plateau.placerTuile(t, p);
    if (carrieresCouvertes != -1)
    {
        setNbrPierres(getNbrPierres() + carrieresCouvertes);
        setNbrPoints();
    }
    return carrieresCouvertes != -1;
}