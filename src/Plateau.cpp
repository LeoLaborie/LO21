#include "Plateau.h"

#include <algorithm>
#include <unordered_map>
#include <utility>

static void reinitialiserParentsTuiles(std::vector<Tuile>& tuiles)
{
    for (Tuile& tuile : tuiles)
        for (Tuile::Iterator it = tuile.getIterator(); !it.isDone(); it.next())
            it.currentItem().setParent(&tuile);
}

Plateau::Plateau(const bool vs[5])
{
    for (int i = 0; i < 5; i++)
    {
        variantesScores[i] = vs[i];
    }
    listeTuiles.clear();
    Tuile tuileDepart{new Hexagone(0, 0, 0, TypeHex::PHabitation), new Hexagone(-1, 1, 0, TypeHex::Carriere), new Hexagone(0, -1, 0, TypeHex::Carriere), new Hexagone(1, 0, 0, TypeHex::Carriere)};
    listeTuiles.push_back(tuileDepart);
    reinitialiserParentsTuiles(listeTuiles);

    updateVoisins();
}

Plateau::Plateau()
{
    listeTuiles.clear();
    Tuile tuileDepart{new Hexagone(0, 0, 0, TypeHex::PHabitation), new Hexagone(-1, 1, 0, TypeHex::Carriere), new Hexagone(0, -1, 0, TypeHex::Carriere), new Hexagone(1, 0, 0, TypeHex::Carriere)};
    listeTuiles.push_back(tuileDepart);
    reinitialiserParentsTuiles(listeTuiles);
}

Plateau Plateau::fromSave(const bool variantes[5], std::vector<Tuile> tuiles)
{
    Plateau p(variantes);
    p.listeTuiles = std::move(tuiles);
    reinitialiserParentsTuiles(p.listeTuiles);
    p.updateVoisins();
    return p;
}

void Plateau::updateVoisins()
{
    // algo en O(n²), on pouurrait le rendre en O(n), mais vu qu'on a tres peu d'hexagone par plateau le n² n'est pas dérangeant
    // Parcourir toutes les tuiles du plateau
    pourChaqueHexagone([&](Hexagone *hexagone1)
                       { pourChaqueHexagone([&](Hexagone *hexagone2)
                                            {
            int dx = hexagone1->getX() - hexagone2->getX();
            int dy = hexagone1->getY() - hexagone2->getY();

            if (hexagone1 != hexagone2 &&
                ((abs(dx) == 1 && dy == 0) || // voisins horizontaux
                 (dx == 0 && abs(dy) == 1) || // voisins verticaux
                 (dx == 1 && dy == -1) ||     // diagonales valides
                 (dx == -1 && dy == 1)))      // diagonales valides
            {
                // cherche si ils sont deja voisins ou non. Si ils ne le sont pas, -> addVoisin()
                if (std::find(hexagone1->getVoisins().begin(), hexagone1->getVoisins().end(), hexagone2) == hexagone1->getVoisins().end())
                {
                    hexagone1->addVoisin(hexagone2);
                }
                if (std::find(hexagone2->getVoisins().begin(), hexagone2->getVoisins().end(), hexagone1) == hexagone2->getVoisins().end())
                {
                    hexagone2->addVoisin(hexagone1);
                }
            } }); });
}

template <class T>
bool ContientPas(const std::vector<T> &v, const T &valeur)
{
    return std::find(v.begin(), v.end(), valeur) == v.end();
}

bool Plateau::verifierPlacementTuile(const Position& p, const Tuile& t) const
{
    return verifierPlacementTuile(p, t, nullptr);
}

bool Plateau::verifierPlacementTuile(const Position &p, const Tuile &t, std::string* raisonEchec) const
{
    auto refuser = [&](std::string message)
    {
        if (raisonEchec)
            *raisonEchec = std::move(message);
        return false;
    };

    std::vector<const Tuile *> tuiles_en_dessous;
    bool surElever = false;
    bool toucheParBord = false;
    int supports_par_hex = 0;  // pour vérifier qu’on pose bien sur 3 hexagones en hauteur

    int dx[6] = {+1, +1, 0, -1, -1, 0};
    int dy[6] = {0, -1, -1, 0, +1, +1};

    struct coord
    {
        int x;
        int y;
        int z;
    };

    // coordonnées des hex à tester pour la tuile posée
    std::vector<coord> coords;
    coords.reserve(t.getNbHexa());
    for (const auto &o : t.getOffsets())
        coords.push_back({p.x + o.q, p.y + o.r, p.z});

    for (const auto &h : coords)
    {
        bool supportTrouve = false;
        bool hexDejaComptePourBord = false;  // pour ne compter chaque hex qu'une seule fois
        bool superposition = false;

        if (h.z > 0)
            surElever = true;

        pourChaqueHexagone([&](const Hexagone *hex_plateau)
                           {
            if (superposition)
                return;

            // on vérifie si deux tuiles ne se superposent pas
            if (h.x == hex_plateau->getX() && h.y == hex_plateau->getY() && h.z == hex_plateau->getZ())
            {
                superposition = true;
                return;
            }

            // on récupère les tuiles en dessous si on est à un niveau supérieur à 0
            // et on garde que les tuiles différentes
            if (!supportTrouve && h.z > 0 && h.x == hex_plateau->getX() && h.y == hex_plateau->getY() && (h.z - hex_plateau->getZ()) == 1)
            {
                supportTrouve = true;
                ++supports_par_hex; // on compte le support pour cet hexagone
                const Tuile *parent = hex_plateau->getParent();
                if (ContientPas(tuiles_en_dessous, parent))
                    tuiles_en_dessous.push_back(parent);
            }

            // on vérifie si on touche par le bord une tuile du plateau (si on est au niveau 0)
            if (h.z == 0 && !hexDejaComptePourBord && hex_plateau->getZ() == 0)
            {
                for (int i = 0; i < 6; ++i)
                {
                    if (h.x + dx[i] == hex_plateau->getX() && h.y + dy[i] == hex_plateau->getY())
                    {
                        toucheParBord=true;
                        hexDejaComptePourBord = true;
                        break;
                    }
                }
            } });

        if (superposition)
            return refuser("Superposition : la case (" + std::to_string(h.x) + "," + std::to_string(h.y) + "," + std::to_string(h.z) + ") est déjà occupée.");

        if (h.z > 0 && !supportTrouve)
            return refuser("Placement en hauteur impossible : pas de support sous (" + std::to_string(h.x) + "," + std::to_string(h.y) + "," + std::to_string(h.z) + ").");
    }

    if (surElever)
    {
        // on pose bien sur 3 hexagones (un support par hex) ET sur au moins 2 tuiles différentes
        if (supports_par_hex != (int)coords.size())
            return refuser("Placement en hauteur impossible : la tuile ne repose pas sur 3 hexagones.");
        if (tuiles_en_dessous.size() < 2)
            return refuser("Placement en hauteur impossible : la tuile doit chevaucher au moins 2 tuiles différentes.");
    }
    else
    {
        if (!toucheParBord)
            return refuser("Placement au sol invalide : la tuile doit être adjacente par un bord à une tuile déjà posée au niveau 0.");
    }

    return true;
}

std::vector<Position> Plateau::getPositionsLegales(const Tuile &t) const
{
    std::vector<Position> listeValide;
    if (listeTuiles.empty())
        return listeValide;

    // On récupère le min et le max
    Tuile::ConstIterator it = listeTuiles[0].getConstIterator();
    const Hexagone &firstHex = it.currentItem();

    int minX = firstHex.getX();
    int maxX = firstHex.getX();
    int minY = firstHex.getY();
    int maxY = firstHex.getY();
    int maxZ = firstHex.getZ();

    pourChaqueHexagone([&](const Hexagone *h)
                       {
        minX = std::min(minX, h->getX());
        maxX = std::max(maxX, h->getX());
        minY = std::min(minY, h->getY());
        maxY = std::max(maxY, h->getY());
        maxZ = std::max(maxZ, h->getZ()); });
    int marge = 3;

    // on se laisse de la marge >1 pour ne pas oublier des positions légales si on prend que 1
    int minXTest = minX - marge;
    int maxXTest = maxX + marge;
    int minYTest = minY - marge;
    int maxYTest = maxY + marge;
    int minZTest = 0;
    int maxZTest = maxZ + 1;

    // on parcours tt les positions possibles peu d'hexagones donc o(n³) ne pose pas de problème
    for (int z = minZTest; z <= maxZTest; ++z)
    {
        for (int x = minXTest; x <= maxXTest; ++x)
        {
            for (int y = minYTest; y <= maxYTest; ++y)
            {
                Position p{x, y, z};
                if (verifierPlacementTuile(p, t))
                {
                    listeValide.push_back(p);
                }
            }
        }
    }

    return listeValide;
}

void Plateau::afficherPositionsLegales(const Tuile &t) const
{
    auto positions = getPositionsLegales(t);

    std::cout << "Positions legales pour cette tuile ("
              << positions.size() << " possibilités) :" << std::endl;

    for (const auto &p : positions)
    {
        std::cout << "  - (" << p.x << ", " << p.y << ", " << p.z << ")\n";
    }
}

int Plateau::placerTuile(Tuile &t, Position &p)
{
    int res = 0;

    std::string raison;
    if (!verifierPlacementTuile(p, t, &raison))
    {
        if (raison.empty())
            raison = "Placement de tuile invalide.";
        throw std::invalid_argument(raison);
    }

    // positionner la tuile (3 hexagones)
    for (Tuile::Iterator it = t.getIterator(); !it.isDone(); it.next())
    {
        auto *h = &it.currentItem();
        const auto &o = t.getOffsets()[it.currentIndex()];
        h->setCoord(p.x + o.q, p.y + o.r, p.z);
    }

    // Insérer la tuile dans le plateau
    listeTuiles.push_back(t);
    reinitialiserParentsTuiles(listeTuiles);
    updateVoisins();

    // Si on est en hauteur (z > 0), on recouvre ce qui est juste en dessous (z-1)
    if (p.z > 0)
    {
        auto Recouvrir = [&](int x, int y)
        {
            bool traite = false;
            pourChaqueHexagone([&](Hexagone *h)
                               {
                if (traite)
                    return;

                if (h->getX() == x && h->getY() == y && h->getZ() == p.z - 1)
                {
                    if (!h->getEstRecouvert())
                    {
                        h->setEstRecouvert();
                        if (h->getType() == TypeHex::Carriere)
                            ++res; // si on trouve une carrière en dessous, on signifie qu'on a recouvert une carrière en ajoutant 1 à la valeur de retour
                    }
                    traite = true;
                } });
        };

        for (const auto &o : t.getOffsets())
            Recouvrir(p.x + o.q, p.y + o.r);
    }

    return res;
}

int Plateau::placerTuile(Tuile &t)
{
    listeTuiles.push_back(t);
    reinitialiserParentsTuiles(listeTuiles);
    return 1;
}

int Plateau::calculerPoints() const
{
    int PlaceHabitation = 0, PlaceMarche = 0, PlaceCaserne = 0, PlaceTemple = 0, PlaceJardin = 0,
        nbMarche = 0, nbCaserne = 0, nbTemple = 0, nbJardin = 0, nbHabitation;
    int total = 0;
    std::vector<SommetHab> grapheHabitation;
    std::unordered_map<const Hexagone *, int> indexHabitation;
    pourChaqueHexagone([&](const Hexagone *h)
                       {
                           switch (h->getType())
                           {
                           case TypeHex::Habitation:
                           {
                               SommetHab current;
                               current.current = h;
                               current.dejaVisite = false;
                               grapheHabitation.push_back(current);
                               indexHabitation[h] = static_cast<int>(grapheHabitation.size() - 1);
                               break;
                           }
                           case TypeHex::Marche:
                           {
                               nbMarche = calculerPointsMarche(h);
                               break;
                           }
                           case TypeHex::Temple:
                           {
                               nbTemple = calculerPointsTemple(h);
                               break;
                           }
                           case TypeHex::Caserne:
                           {
                               nbCaserne = calculerPointsCaserne(h);
                               break;
                           }
                           case TypeHex::Jardin:
                           {
                               nbJardin = calculerPointsJardin(h);
                               break;
                           }
                           case TypeHex::PHabitation:
                           {
                               PlaceHabitation++;
                               break;
                           }
                           case TypeHex::PMarche:
                           {
                               PlaceMarche += 2;
                               break;
                           }
                           case TypeHex::PCaserne:
                           {
                               PlaceCaserne += 2;
                               break;
                           }
                           case TypeHex::PTemple:
                           {
                               PlaceTemple += 2;
                               break;
                           }
                           case TypeHex::PJardin:
                           {
                               PlaceJardin += 3;
                               break;
                           }
                           default:
                               break;
                           } });
    for (auto &sommet : grapheHabitation)
    {
        for (auto voisin : sommet.current->getVoisins())
        {
            auto it = indexHabitation.find(voisin);
            if (it != indexHabitation.end())
            {
                sommet.voisins.push_back(it->second);
            }
        }
    }
    nbHabitation = calculerPointsHabitation(grapheHabitation);
    total += nbHabitation * PlaceHabitation + nbMarche * PlaceMarche + nbCaserne * PlaceCaserne + nbTemple * PlaceTemple + nbJardin * PlaceJardin;
    return total;
}

int Plateau::calculerPointsia(int &diff) const
{
    int PlaceHabitation = 0, PlaceMarche = 0, PlaceCaserne = 0, PlaceTemple = 0, PlaceJardin = 0;
    int nbHabitation = 0, nbMarche = 0, nbCaserne = 0, nbTemple = 0, nbJardin = 0, nbCarriere = 0;
    int total = 0;

    pourChaqueHexagone([&](const Hexagone *h)
                       {
                           switch (h->getType())
                           {
                           case TypeHex::Habitation:
                               nbHabitation++;
                               break;
                           case TypeHex::Marche:
                               nbMarche++;
                               break;
                           case TypeHex::Temple:
                               nbTemple++;
                               break;
                           case TypeHex::Caserne:
                               nbCaserne++;
                               break;
                           case TypeHex::Jardin:
                               nbJardin++;
                               break;
                           case TypeHex::PHabitation:
                               PlaceHabitation++;
                               break;
                           case TypeHex::PMarche:
                               PlaceMarche += 2;
                               break;
                           case TypeHex::PCaserne:
                               PlaceCaserne += 2;
                               break;
                           case TypeHex::PTemple:
                               PlaceTemple += 2;
                               break;
                           case TypeHex::PJardin:
                               PlaceJardin += 3;
                               break;
                           case TypeHex::Carriere:
                               nbCarriere += 2;
                               break;
                           default:
                               break;
                           } });
    if (diff == 2)
    {
        total += nbCarriere;
    }
    if (diff == 3)
    {
        nbHabitation *= 2;
        nbMarche *= 2;
        nbCaserne *= 2;
        nbTemple *= 2;
        nbJardin *= 2;
    }
    total += nbHabitation * PlaceHabitation + nbMarche * PlaceMarche + nbCaserne * PlaceCaserne + nbTemple * PlaceTemple + nbJardin * PlaceJardin;
    return total;
}

int Plateau::calculerPointsCaserne(const Hexagone *h) const
{
    int multi = 1;
    if (variantesScores[2] && h->getVoisins().size() <= 3)
        multi = 2;
    return (h->getVoisins().size() <= 5) ? (h->getZ() + 1) * multi : 0;
}

int Plateau::calculerPointsTemple(const Hexagone *h) const
{
    int mult = 1;
    if (variantesScores[3] && h->getZ() >= 1)
        mult = 2;
    return (h->getVoisins().size() == 6) ? (h->getZ() + 1) * mult : 0;
}

int Plateau::calculerPointsJardin(const Hexagone *h) const
{
    return (h->getZ() + 1) + conditionVarianteJardin(h) * variantesScores[4] * (h->getZ() + 1);
}

int Plateau::calculerPointsMarche(const Hexagone *h) const
{
    int nbpoint = 0;
    bool voisinMarche = false;
    for (const auto &voisin : h->getVoisins())
    {
        if (voisin->getType() == TypeHex::Marche)
        {
            voisinMarche = true;
            break;
        }
    }
    if (!voisinMarche)
    {
        nbpoint += h->getZ();

        if (variantesScores[1])
        {
            for (const auto &voisin : h->getVoisins())
            {
                if (voisin->getType() == TypeHex::PMarche)
                {
                    nbpoint += h->getZ();
                    break;
                }
            }
        }
    }
    return nbpoint;
}

int Plateau::calculerPointsHabitation(std::vector<SommetHab> grapheHabitation) const
{
    int plusGrosQuartier = 0;
    int scorePlusgrosQuartier = 0;
    for (std::size_t i = 0; i < grapheHabitation.size(); ++i)
    {
        if (grapheHabitation[i].dejaVisite)
            continue;

        int tailleQuartier = 0;
        int scoreQuartier = 0;
        std::vector<std::size_t> aVisiter = {i};
        grapheHabitation[i].dejaVisite = true;

        while (!aVisiter.empty())
        {
            std::size_t currentIndex = aVisiter.back();
            aVisiter.pop_back();
            tailleQuartier += 1;
            scoreQuartier += grapheHabitation[currentIndex].current->getZ() + 1;

            for (int voisinIndex : grapheHabitation[currentIndex].voisins)
            {
                std::size_t voisin = voisinIndex;
                if (!grapheHabitation[voisin].dejaVisite)
                {
                    grapheHabitation[voisin].dejaVisite = true;
                    aVisiter.push_back(voisin);
                }
            }
        }
        plusGrosQuartier = std::max(plusGrosQuartier, tailleQuartier);
        if (tailleQuartier == plusGrosQuartier && scorePlusgrosQuartier < scoreQuartier)
            scorePlusgrosQuartier = scoreQuartier;
    }
    if (scorePlusgrosQuartier >= 10 && variantesScores[0])
        scorePlusgrosQuartier *= 2;
    return scorePlusgrosQuartier;
}

bool Plateau::conditionVarianteJardin(const Hexagone *q) const
{
    bool conditionRemplie = 0;
    bool estVide, estLac = true;

    std::vector<std::vector<int>> coVoisinsRelatifs = {{0, -1}, {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}};
    std::vector<std::vector<int>> coVoisins = {};
    std::vector<std::vector<int>> coVoisinsVides = {};

    // On trouve les coordonnées relatives des voisins du jardin
    pourChaqueHexagone([&](const Hexagone *h)
                       {
        for (const auto &co : coVoisinsRelatifs)
        {
            if ((q->getX() + co[0]) == h->getX() && (q->getY() + co[1]) == h->getY())
            { // Ne pas hésiter à me redemander (Dimitri)
                coVoisins.push_back(co);
            }
        } });

    // On recupère les voisins vides du jardin par différence avec les coordonnées relatives de des voisins du jardin
    for (const auto &cor : coVoisinsRelatifs)
    {
        estVide = true;
        for (const auto &cov : coVoisins)
        {
            if (cor == cov)
            {
                estVide = false;
            }
        }
        if (estVide)
        {
            coVoisinsVides.push_back(cor);
        }
    }

    while (estLac && coVoisinsVides.size() > 0)
    {
        std::vector<int> vide = coVoisinsVides[0];
        for (std::vector<int> co : coVoisinsRelatifs)
        {
            bool aVoisinNonVide = false;
            pourChaqueHexagone([&](const Hexagone *h)
                               {
                if (vide[0] + co[0] == h->getX() && vide[1] + co[1] == h->getY()){
                    aVoisinNonVide = true;
                } });
            if (!aVoisinNonVide)
            {
                estLac = false;
            }
        }
        coVoisinsVides.erase(coVoisinsVides.begin());
    }

    return conditionRemplie;
}

std::ostream &operator<<(std::ostream &os, const Plateau &p)
{
    os << "\nPlateau contient " << p.listeTuiles.size() << " tuiles :\n";
    os << " ----\n";
    for (const auto &t : p.listeTuiles)
    {
        t.afficherDetails();
        os << "----\n";
    }
    return os;
}
