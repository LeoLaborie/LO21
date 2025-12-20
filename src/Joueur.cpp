#include "Joueur.h"

#include <limits>
#include <stdexcept>
#include <utility>

Joueur::Joueur(const bool variantesScore[5], std::string nom)
    : nbrPierres(0), nbrPoints(0), nom(std::move(nom)), plateau(variantesScore) {}

Joueur::Joueur(std::string nom)
    : nbrPierres(0), nbrPoints(0), nom(std::move(nom)), plateau() {}

IllustreArchitecte::IllustreArchitecte(int diff)
    : Joueur("Illustre Architecte"), difficulte(diff) {};

Joueur::Joueur(const bool variantes[5], std::string nomSave, int pierres,
               int points, Tuile tuileMain, std::vector<Tuile> plateauSave)
    : nbrPierres(pierres), nbrPoints(points), nom(std::move(nomSave)),
      plateau(Plateau::fromSave(variantes, std::move(plateauSave))),
      tuileEnMain(std::move(tuileMain)) {}

Joueur Joueur::fromSave(const bool variantes[5], std::string nom, int pierres,
                        int points, Tuile tuileMain,
                        std::vector<Tuile> plateau)
{
    return Joueur(variantes, std::move(nom), pierres, points,
                  std::move(tuileMain), std::move(plateau));
}

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
void Joueur::setNbrPointsFinal()
{
    nbrPoints = getPlateau().calculerPoints() + getNbrPierres();
}

void IllustreArchitecte::setNbrPoints()
{
    nbrPoints = getPlateau().calculerPointsia(getdifficulte()) + getNbrPierres();
}

Tuile &Joueur::piocherTuile(TuileId id, Chantier &chantier, Joueur *fauxJoueur)
{
    const int indice = chantier.indexOf(id);
    if (indice < 0)
        throw std::out_of_range("ID de tuile invalide.");
    if (indice > getNbrPierres())
        throw std::invalid_argument("Nombre de pierres insuffisant.");
    setNbrPierres(getNbrPierres() - indice);
    if (fauxJoueur)
    {
        fauxJoueur->setNbrPierres(fauxJoueur->getNbrPierres() + indice);
    }
    const Tuile *tuile = chantier.trouverParId(id);
    if (!tuile)
        throw std::out_of_range("ID de tuile invalide.");
    setTuileEnMain(*tuile);
    chantier.retirerTuile(id);
    return tuileEnMain;
}

Tuile &IllustreArchitecte::piocherTuile(TuileId id, Chantier &chantier, Joueur *fauxjoueur)
{
    (void)fauxjoueur; // pour enlever warning et garder polymorphisme
    const int indice = chantier.indexOf(id);
    if (indice < 0)
        throw std::out_of_range("ID de tuile invalide.");
    setNbrPierres(getNbrPierres() - indice);
    const Tuile *tuile = chantier.trouverParId(id);
    if (!tuile)
        throw std::out_of_range("ID de tuile invalide.");
    setTuileEnMain(*tuile);
    chantier.retirerTuile(id);
    return tuileEnMain;
}

void Joueur::placerTuile(Tuile &t, Position &p)
{
    try
    {
        int carrieresCouvertes = plateau.placerTuile(t, p);
        setTuileEnMain(Tuile());
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
    int idTuile = -1;
    while (idTuile < 0 || static_cast<size_t>(idTuile) >= chantier.getTaille())
    {
        std::cout << "Entrez l'ID de la tuile à piocher : ";

        if (!(std::cin >> idTuile))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            idTuile = -1;
        }
        else if (idTuile < 0 || static_cast<size_t>(idTuile) >= chantier.getTaille())
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
            std::cout
                << "Vous n'avez pas assez de pierres pour piocher cette tuile.\n";
            texte_reset();
            idTuile = -1; // Réinitialiser pour redemander
        }
    }
    return idTuile;
}

int IllustreArchitecte::choixTuile(const Chantier &chantier)
{
    int idTuile = -1;
    const std::vector<Tuile> &tuiles = chantier.getTuiles();
    size_t i = 0;
    do
    {
        for (Tuile::ConstIterator it = tuiles[i].getConstIterator(); !it.isDone(); it.next())
        {
            const Hexagone &h = it.currentItem();
            if ((h.getType() == TypeHex::PHabitation ||
                 h.getType() == TypeHex::PCaserne ||
                 h.getType() == TypeHex::PTemple ||
                 h.getType() == TypeHex::PMarche ||
                 h.getType() == TypeHex::PJardin) &&
                (getNbrPierres() >= static_cast<int>(i)))
            {
                return static_cast<int>(i);
            }
        }
        i++;
    } while (i < tuiles.size() && idTuile == -1 && getNbrPierres() >= static_cast<int>(i));
    if (idTuile == -1)
    {
        idTuile = 0;
    }
    return idTuile;
}

IllustreArchitecte *IllustreArchitecte::fromSave(int diff, int pierres, int points,
                                                 const bool variantes[5],
                                                 std::vector<Tuile> plateauSave)
{
    auto *ia = new IllustreArchitecte(diff);
    ia->nbrPierres = pierres;
    ia->nbrPoints = points;
    ia->plateau = Plateau::fromSave(variantes, std::move(plateauSave));
    return ia;
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

bool Joueur::tuileDejaEnMain() const
{
    Tuile::ConstIterator it = tuileEnMain.getConstIterator();
    return !it.isDone();
}
