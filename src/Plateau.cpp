#include "Plateau.h"

Plateau::Plateau()
{
    listeTuiles.clear();
    listeHexagones = {
        // Tuile de départ au niveau 0 (z = 0)
        new Place(0, 0, 0, TypePlace::Habitation, 1),
        new Carriere(-1, 1, 0),
        new Carriere(0, -1, 0),
        new Carriere(1, 0, 0)};

    updateVoisins();
}

void Plateau::updateVoisins()
{
    // algo en O(n²), on pouurrait le rendre en O(n), mais vu qu'on a tres peu d'hexagone par plateau le n² n'est pas dérangeant
    // Parcourir toutes les tuiles du plateau
    for (auto *&hexagone1 : listeHexagones)
    {
        for (auto *&hexagone2 : listeHexagones)
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
            }
        }
    }
}

template <class T>
bool ContientPas(const std::vector<T> &v, const T &valeur)
{
    return std::find(v.begin(), v.end(), valeur) == v.end();
}

bool Plateau::verifierPlacementTuile(C &c) const
{
    std::vector<Tuile *> tuiles_en_dessous;
    bool surElever = false;
    bool toucheParBord = false;

    int dx[6] = {+1, +1, 0, -1, -1, 0};
    int dy[6] = {0, -1, -1, 0, +1, +1};

    struct coord
    {
        int x;
        int y;
        int z;
    };
    coord coords[3];
    coords[0].x = c.x; // hex0
    coords[0].y = c.y;
    coords[0].z = c.z;
    coords[1].x = c.x - 1; // hex1
    coords[1].y = c.y + 1;
    coords[1].z = c.z;
    coords[2].x = c.x;
    coords[2].y = c.y + 1;
    coords[2].z = c.z;
    for (const auto &h : coords)
    {
        bool supportTrouve = false;

        if (h.z > 1)
            surElever = true;

        for (auto *hex_plateau : listeHexagones)
        {
            // pn vérifie si deux tuiles ne se superposent pas
            if (h.x == hex_plateau->getX() && h.y == hex_plateau->getY() && h.z == hex_plateau->getZ())
                return false;

            // on récupère les tuiles en dessous si on est à un niveau supérieur à 1
            // et on garde que les tuiles différentes
            if (h.x == hex_plateau->getX() && h.y == hex_plateau->getY() && (h.z - hex_plateau->getZ()) == 1)
            {
                supportTrouve = true;
                Tuile *parent = hex_plateau->getParent();
                if (ContientPas(tuiles_en_dessous, parent))
                    tuiles_en_dessous.push_back(parent);
            }

            // on vérifie si on touche par le bord une tuile du plateau (si on est au niveau 0)
            if (h.z == 1)
            {
                for (int i = 0; i < 6; ++i)
                {
                    if (h.x + dx[i] == hex_plateau->getX() && h.y + dy[i] == hex_plateau->getY() && hex_plateau->getZ() == 0)
                    {
                        toucheParBord = true;
                        break;
                    }
                }
            }
        }

        if (h.z > 1 && !supportTrouve)
            return false;
    }
    if (surElever)
    {
        if (tuiles_en_dessous.size() < 2)
            return false;
    }
    else
    {
        if (!toucheParBord)
            return false;
    }

    return true;
}

C *Plateau::essayerPlacerTuile(Tuile &t)
{
    // 1) on balaie une grille autour de (0,0,0)
    auto grille = Plateau::grillePetite(3);

    // 2) on teste plusieurs orientations (0,1,2 rotations)
    Tuile tuileMutable = t; // on copie pour pouvoir pivoter
    for (int rot = 0; rot < 3; ++rot)
    {
        if (rot > 0)
            tuileMutable.pivoterTuile();

        for (const auto &c : grille)
        {
            C tempC{c};
            if (verifierPlacementTuile(tempC))
            {
                std::cout << "    -> placement OK en (" << c.x << "," << c.y << "," << c.z
                          << ") avec rotation=" << rot << "\n";
                Tuile tuileAPlacer = tuileMutable; // copie de l’orientation courante
                return new C{c.x, c.y, c.z};
            }
        }
    }
    return nullptr;
}

bool Plateau::ajouterTuile(Tuile &t, C &c)
{
    bool res = false;
    if (!verifierPlacementTuile(c))
    {
        std::cout << "Placement de tuile invalide." << std::endl;
        return false;
    }
    if (c.z > 1)
    {
        auto *h0 = t.getHexagones()[0];
        auto *h1 = t.getHexagones()[1];
        auto *h2 = t.getHexagones()[2];

        h0->SetCoord(c.x, c.y, c.z);
        h1->SetCoord(c.x - 1, c.y + 1, c.z);
        h2->SetCoord(c.x, c.y + 1, c.z);
        // Insérer la tuile dans le plateau
        listeTuiles.push_back(t);
        updateVoisins();

        std::for_each(listeHexagones.begin(), listeHexagones.end(), [&](Hexagone *h)
                      {
        if (h->getX() == c.x && h->getY() == c.y  && !h->getEstRecouvert()) {
            h->setEstRecouvert();
            if (dynamic_cast<const Carriere*>(h)){
                res = true;
            }
        } });
        std::for_each(listeHexagones.begin(), listeHexagones.end(), [&](Hexagone *h)
                      {
        if (h->getX() == c.x - 1 && h->getY() == c.y + 1 && !h->getEstRecouvert()){
            h->setEstRecouvert();
            if (dynamic_cast<const Carriere*>(h)){
                res = true;
            }
        } });
        std::for_each(listeHexagones.begin(), listeHexagones.end(), [&](Hexagone *h)
                      {
        if (h->getX() == c.x && h->getY() == c.y + 1 && !h->getEstRecouvert()) {
            h->setEstRecouvert();
            if (dynamic_cast<const Carriere*>(h)){
                res = true;
            }
        } });
    }
    return res;
}

int Plateau::calculerPoints() const
{

    int placeHabitation = 0;
    int placeMarche = 0;
    int placeTemple = 0;
    int placeCaserne = 0;
    int placeJardin = 0;

    int nbJardin = 0;
    int nbCaserne = 0;
    int nbTemple = 0;
    int nbHabitation = 0;
    int nbMarche = 0;

    std::vector<const Quartier *> tabHabitation; // pour stocker toutes les habitations, on détermine le plus grand groupe après

    for (const Hexagone *hex : listeHexagones)
    {
        if (hex->getEstRecouvert())
            continue;

        if (hex->getEstRecouvert())
            continue;

        if (const Quartier *q = dynamic_cast<const Quartier *>(hex))
        {
            if (q->getTypeQuartier() == TypeQuartier::Jardin)
                nbJardin += q->getZ();

            if (q->getTypeQuartier() == TypeQuartier::Caserne)
            {
                if (q->getVoisins().size() <= 3)
                    nbCaserne += q->getZ(); // si il a 3 voisins ou moins c'est qu'il est sur un bord
            }

            if (q->getTypeQuartier() == TypeQuartier::Temple)
            {
                if (q->getVoisins().size() == 4)
                    nbTemple += q->getZ();
            }
            if (q->getTypeQuartier() == TypeQuartier::Marche)
            {
                for (const auto &voisin : q->getVoisins())
                {
                    if (dynamic_cast<const Quartier *>(voisin)->getTypeQuartier() == TypeQuartier::Marche)
                    {
                        continue;
                    }
                }
                nbMarche += q->getZ();
            }
            if (q->getTypeQuartier() == TypeQuartier::Habitation)
            {
                tabHabitation.push_back(q);
            }
        }
        if (const Place *p = dynamic_cast<const Place *>(hex))
        {
            switch (p->getTypePlace())
            {
            case TypePlace::Habitation:
                placeHabitation += p->getMultiplicateur();
                break;
            case TypePlace::Marche:
                placeMarche += p->getMultiplicateur();
                break;
            case TypePlace::Temple:
                placeTemple += p->getMultiplicateur();
                break;
            case TypePlace::Caserne:
                placeCaserne += p->getMultiplicateur();
                break;
            case TypePlace::Jardin:
                placeJardin += p->getMultiplicateur();
                break;
            }
            continue;
        }
    }
    // On détermine le plus grand groupe d'habitations
    std::vector<const Quartier *> groupeMaxHab; // le groupe d'habitation le plus grand, la taille est à 0 par défaut
    std::vector<const Quartier *> habVisites;   // liste des habitations déjà visités

    for (const Quartier *hab : tabHabitation)
    {

        if (ContientPas(habVisites, hab))
        {

            std::vector<const Quartier *> voisinsHabitation; // les voisins, que l'on va ajouter et supprimer au fur et à mesure
            voisinsHabitation.push_back(hab);

            habVisites.push_back(hab);

            std::vector<const Quartier *> groupeHabitation; // le groupe d'habitation que l'on est en train de regarder
            groupeHabitation.push_back(hab);

            while (voisinsHabitation.size())
            { // on parcourt les voisins jusqu'à ce qu'il n'y en ait plus

                const Quartier *habActuelle = voisinsHabitation.back();
                voisinsHabitation.pop_back();

                for (const Hexagone *voisin : habActuelle->getVoisins())
                {

                    if (const Quartier *quartierVoisin = dynamic_cast<const Quartier *>(voisin))
                    {
                        // On vérifie que le voisin est un quartier (si la conversion marche, c'est le cas, sinon ça renvoie nullprt)

                        if ((quartierVoisin->getTypeQuartier() == TypeQuartier::Habitation) && ContientPas(habVisites, quartierVoisin))
                        {
                            // On vérifie que le quartier voisin n'a pas déjà été visité et que c'est bien une habitation

                            voisinsHabitation.push_back(quartierVoisin);
                            groupeHabitation.push_back(quartierVoisin);
                            habVisites.push_back(quartierVoisin);
                        }
                    }
                }
            }

            if (groupeHabitation.size() > groupeMaxHab.size())
            {
                groupeMaxHab = groupeHabitation;
            }
        }
    }

    for (const Quartier *hab : groupeMaxHab)
    {
        nbHabitation += hab->getZ();
    }

    return placeCaserne * nbCaserne + placeHabitation * nbHabitation + placeJardin * nbJardin + placeMarche * nbMarche + placeTemple * nbTemple;
}

void Plateau::afficher() const
{
    std::cout << "  Hexagones sur plateau: " << getHexagones().size() << "\n";
    for (auto *h : getHexagones())
    {
        std::cout << "    (" << h->getX() << "," << h->getY() << "," << h->getZ() << ")"
                  << " voisins=" << h->getVoisins().size()
                  << (h->getEstRecouvert() ? " [recouvert]" : "") << "\n";
    }
}

std::vector<C> Plateau::grillePetite(int r)
{
    std::vector<C> res;
    for (int x = -r; x <= r; ++x)
    {
        for (int y = -r; y <= r; ++y)
        {
            int z = -x - y;
            if (std::abs(z) > r)
                continue;
            res.push_back({x, y, z});
        }
    }
    return res;
}