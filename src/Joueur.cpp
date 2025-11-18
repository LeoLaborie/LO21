#include "Joueur.h"
#include <utility>
#include <limits>

Joueur::Joueur(const bool variantesScore[5], std::string nom)
    : nbrPierres(0), nbrPoints(0), nom(std::move(nom)), plateau(variantesScore) {}

Joueur::Joueur(std::string nom)
    : nbrPierres(0), nbrPoints(0), nom(std::move(nom)), plateau() {}

IllustreArchitecte::IllustreArchitecte(int diff)
    : Joueur("Illustre Architecte"),
      difficulte(diff) {};

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

void IllustreArchitecte::setNbrPoints()
{
    nbrPoints = getPlateau().calculerPoints();
}

Tuile *Joueur::piocherTuile(int id, Chantier &chantier, IllustreArchitecte *fauxJoueur)
{
    if (id < 0 || id >= chantier.getTaille())
        return nullptr;
    if (id > getNbrPierres())
        return nullptr;
    setNbrPierres(getNbrPierres() - id);
    if (fauxJoueur)
    {
        fauxJoueur->setNbrPierres(fauxJoueur->getNbrPierres() + id);
    }
    setTuileEnMain(chantier.getTuiles()[id]);
    chantier.retirerTuile(id);
    return &tuileEnMain;
}

Tuile *IllustreArchitecte::piocherTuile(int id, Chantier &chantier)
{
    setNbrPierres(getNbrPierres() - id);
    setTuileEnMain(chantier.getTuiles()[id]);
    chantier.retirerTuile(id);
    return &tuileEnMain;
}

void Joueur::placerTuile(Tuile &t, Position &p)
{
    try
    {
        int carrieresCouvertes = plateau.placerTuile(t, p);
        if (carrieresCouvertes != -1)
        {
            setNbrPierres(getNbrPierres() + carrieresCouvertes);
            setNbrPoints();
        }
    }
    catch (const std::invalid_argument &e)
    {
        throw e;
    }
}

void IllustreArchitecte::placerTuile(Tuile &t)
{
    plateau.placerTuile(t);
    setNbrPoints();
}

int Joueur::choixTuile(const Chantier &chantier)
{
    /* Permet au joueur, que ce soit un vrai joueur ou le faux joueur, de piocher une tuile dans le chantier
     *@param le chantier de la partie
     *@return l'id de la Tuile qui a été pioché par le joueur
     */
    int idTuile = -1;
    while (chantier.getTaille() <= idTuile || idTuile < 0)
    {
        std::cout << "Entrez l'ID de la tuile à piocher : ";

        if (!(std::cin >> idTuile))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            idTuile = -1;
        }
        else if (idTuile < 0 || idTuile >= chantier.getTaille())
        {
            texte_couleur(ROUGE);
            texte_gras_on();
            std::cout << "ID invalide. Veuillez réessayer.\n";
            texte_reset();
            idTuile = -1; // Réinitialiser pour redemander
        }
        else if (idTuile > getNbrPierres())
        {
            texte_couleur(ROUGE);
            texte_gras_on();
            std::cout << "Vous n'avez pas assez de pierres pour piocher cette tuile.\n";
            texte_reset();
            idTuile = -1; // Réinitialiser pour redemander
        }
    }
    return idTuile;
}

int IllustreArchitecte::choixTuile(const Chantier &chantier)
{
    int idTuile = -1;
    const std::vector<Tuile> &tuile = chantier.getTuiles();
    long unsigned int i = 0;
    do
    {
        for (const Hexagone *h : tuile[i].getHexagones())
        {
            if ((h->getType() == TypeHex::PHabitation || h->getType() == TypeHex::PCaserne ||
                 h->getType() == TypeHex::PTemple || h->getType() == TypeHex::PMarche || h->getType() == TypeHex::PJardin) &&
                (getNbrPierres() < idTuile))
            {
                idTuile = i;
            }
        }
        i++;
    } while (i < tuile.size() && idTuile == -1);
    if (idTuile == -1)
    {
        idTuile = 0;
    }
    return idTuile;
}

std::ostream &operator<<(std::ostream &os, const Joueur &j)
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
    os << j.nbrPoints;
    texte_reset();
    os << "\n"
       << j.plateau << std::endl;
    return os;
}
