#include "Joueur.h"

Joueur::Joueur(bool varianteScore,std::string nom): nbrPierres(0),nbrPoints(0),nom(nom),plateau(varianteScore) {};

int Joueur::getNbrPierres() const
{
    return nbrPierres;
}

int Joueur::getNbrPoints() const
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
std::ostream& operator<<(std::ostream &os,const Joueur& j)
{
    os << " ";
    texte_couleur(ROUGE);
    texte_gras_on();
    os << j.nom;
    texte_reset();
    os << " | Pierres : ";
    texte_couleur(BLEU);
    os << j.nbrPierres;
    texte_reset();
    os << ", Points : ";
    texte_couleur(JAUNE);
    os<< j.nbrPoints;
    texte_reset();
    os << "\n"<<j.plateau<<std::endl;
    return os;
    }
