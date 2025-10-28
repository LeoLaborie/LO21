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
    
    Tuile tuileDepart{listeHexagones[0], listeHexagones[1], listeHexagones[2], listeHexagones[3]};
    listeTuiles.push_back(tuileDepart);

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

bool Plateau::verifierPlacementTuile(Position &p) const
{
    std::vector<Tuile *> tuiles_en_dessous;
    bool surElever = false;
    bool toucheParBord = false;
    int supports_par_hex = 0; // nouveau : pour vérifier qu’on pose bien sur 3 hexagones en hauteur

    int dx[6] = {+1, +1, 0, -1, -1, 0};
    int dy[6] = {0, -1, -1, 0, +1, +1};

    struct coord
    {
        int x;
        int y;
        int z;
    };
    coord coords[3];
    coords[0] = {p.x, p.y, p.z};         // hex0
    coords[1] = {p.x - 1, p.y + 1, p.z}; // hex1
    coords[2] = {p.x, p.y + 1, p.z};     // hex2

    for (const auto &h : coords)
    {
        bool supportTrouve = false;

        if (h.z > 0)
            surElever = true;

        for (auto *hex_plateau : listeHexagones)
        {
            // on vérifie si deux tuiles ne se superposent pas
            if (h.x == hex_plateau->getX() && h.y == hex_plateau->getY() && h.z == hex_plateau->getZ())
                return false;

            // on récupère les tuiles en dessous si on est à un niveau supérieur à 0
            // et on garde que les tuiles différentes
            if (h.z > 0 && h.x == hex_plateau->getX() && h.y == hex_plateau->getY() && (h.z - hex_plateau->getZ()) == 1)
            {
                supportTrouve = true;
                ++supports_par_hex; // on compte le support pour cet hexagone
                Tuile *parent = hex_plateau->getParent();
                if (ContientPas(tuiles_en_dessous, parent))
                    tuiles_en_dessous.push_back(parent);
            }

            // on vérifie si on touche par le bord une tuile du plateau (si on est au niveau 0)
            if (h.z == 0)
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

        if (h.z > 0 && !supportTrouve)
            return false; // chaque hex doit avoir son support direct
    }

    if (surElever)
    {
        // on pose bien sur 3 hexagones (un support par hex) ET sur au moins 2 tuiles différentes
        if (supports_par_hex != 3)
            return false;
        if (tuiles_en_dessous.size() < 2)
            return false;
    }
    else
    {
        // au sol, il faut toucher par le bord
        if (!toucheParBord)
            return false;
    }

    return true;
}

int Plateau::placerTuile(Tuile &t, Position &p)
{
    int res = 0;

    if (!verifierPlacementTuile(p))
    {
        texte_couleur(ROUGE);
        texte_gras_on();
        std::cout << " Placement de tuile invalide." << std::endl;
        texte_reset();
        return -1;
    }

    // positionner la tuile (3 hexagones)
    auto *h0 = t.getHexagones()[0];
    auto *h1 = t.getHexagones()[1];
    auto *h2 = t.getHexagones()[2];

    h0->SetCoord(p.x, p.y, p.z);
    h1->SetCoord(p.x - 1, p.y + 1, p.z);
    h2->SetCoord(p.x, p.y + 1, p.z);

    // Insérer la tuile dans le plateau
    listeTuiles.push_back(t);
    for (auto *h : t.getHexagones())
    {
        listeHexagones.push_back(h);
    }

    updateVoisins();

    // Si on est en hauteur (z > 0), on recouvre ce qui est juste en dessous (z-1)
    if (p.z > 0)
    {
        auto Recouvrir = [&](int x, int y)
        {
            for (auto *h : listeHexagones)
            {
                if (h->getX() == x && h->getY() == y && h->getZ() == p.z - 1)
                {
                    if (!h->getEstRecouvert())
                    {
                        h->setEstRecouvert();
                        if (dynamic_cast<const Carriere *>(h))
                            ++res; // si on trouve une carrière en dessous, on signifie qu'on a recouvert une carrière en ajoutant 1 à la valeur de retour
                    }
                    break;
                }
            }
        };

        Recouvrir(p.x, p.y);
        Recouvrir(p.x - 1, p.y + 1);
        Recouvrir(p.x, p.y + 1);
    }

    return res;
}

int Plateau::calculerPoints() const
{
    return calculerPointsCaserne() + calculerPointsHabitation() + calculerPointsJardin() + calculerPointsMarche() + calculerPointsTemple();
}

int Plateau::calculerPointsCaserne() const
{
    int placeCaserne = 0;
    int nbCaserne = 0;

    for (const Hexagone *hex : listeHexagones)
    {
        if (hex->getEstRecouvert())
            continue;

        if (hex->getEstRecouvert())
            continue;

        if (const Quartier *q = dynamic_cast<const Quartier *>(hex))
        {
            if (q->getTypeQuartier() == TypeQuartier::Caserne)
            {
                if (q->getVoisins().size() <= 5)
                    nbCaserne += q->getZ(); // si il a 5 voisins ou moins p'est qu'il est sur un bord

                if  (varianteCaserne && q->getVoisins().size() <= 3)
                    nbCaserne += q->getZ(); // si la variante est activée et qu'il y a moins de 3 voisins ou moins, on doubles le nb de casernes
            }         
        }
        if (const Place *p = dynamic_cast<const Place *>(hex))
        {
            if (p->getTypePlace() == TypePlace::Caserne)
                placeCaserne += p->getMultiplicateur();
        }
    }

    return placeCaserne * nbCaserne;
}

int Plateau::calculerPointsTemple() const
{
    /*
     *Calcul Le nombre de points donnés par les temples (en comprenant les places et les varaiantes)
     *
     *@return Le nombre total de points
     */
    int placeTemple = 0;
    int nbTemple = 0;

    for (const Hexagone *hex : listeHexagones)
    {
        if (hex->getEstRecouvert())
            continue;

        if (hex->getEstRecouvert())
            continue;

        if (const Quartier *q = dynamic_cast<const Quartier *>(hex))
        {
            if (q->getTypeQuartier() == TypeQuartier::Temple)
            {
                if (q->getVoisins().size() == 6)
                    nbTemple += q->getZ();

                if(varianteTemple && q->getZ() > 1)
                    nbTemple += q->getZ(); // On double les points des temples si ils sont à une hauteur plus hate que 1
            }
        }
        if (const Place *p = dynamic_cast<const Place *>(hex))
        {
            if (p->getTypePlace() == TypePlace::Temple)
                placeTemple += p->getMultiplicateur();
        }
    }

    return placeTemple * nbTemple;
}

bool Plateau::conditionVarianteJardin (const Quartier* q) const{
    bool conditionRemplie = 0;

    std::vector<std::vector<int>> coVoisinsRelatifs = {{0, -1}, {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}};
    std::vector<std::vector<int>> coVoisinsVides = {};

    // On trouve les coordonnées relatives vides
    for (const auto h: listeHexagones){
        for (const auto co : coVoisinsRelatifs){
            if((h->getX()-co[0]) == q->getX() && (h->getY()-co[1]) == q->getY()){ // Ne pas hésiter à me redemander (Dimitri) 
                                                                                 //si vous comprenez pas parce que même moi je suis pas sûr
                coVoisinsVides.push_back(co);
            }
        }
    }

    for (const auto co: coVoisinsVides){
        for (const auto h :listeHexagones){
            if((h->getX()-co[0]) == q->getX() && (h->getY()-co[1]) == q->getY()){                                                                
                coVoisinsVides.push_back(co);
            }
        }
    }
    


    return conditionRemplie;
}

int Plateau::calculerPointsJardin() const{

    /*
     *Calcul Le nombre de points donnés par les jardins (en comprenant les places et les varaiantes)
     *
     *@return Le nombre total de points
     */

    int placeJardin = 0;
    int nbJardin = 0;

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
            
            if(varianteJardin && Plateau::conditionVarianteJardin(q))
                nbJardin += q->getZ();
        }

        if (const Place *p = dynamic_cast<const Place *>(hex))
        {
            if (p->getTypePlace() == TypePlace::Jardin)
                placeJardin += p->getMultiplicateur();
        }
    }

    return placeJardin * nbJardin;
}

int Plateau::calculerPointsMarche() const
{
    /*
     *Calcul Le nombre de points donnés par les marchés (en comprenant les places et les varaiantes)
     *
     *@return Le nombre total de points
     */

    int placeMarche = 0;
    int nbMarche = 0;
  
    bool voisinMarche = 0;

    for (const Hexagone *hex : listeHexagones)
    {
        if (hex->getEstRecouvert())
            continue;

        if (hex->getEstRecouvert())
            continue;

        if (const Quartier *q = dynamic_cast<const Quartier *>(hex))
        {
            voisinMarche = 0;
            if (q->getTypeQuartier() == TypeQuartier::Marche)
            {
                for (const auto &voisin : q->getVoisins())
                {
                    if (dynamic_cast<const Quartier *>(voisin)->getTypeQuartier() == TypeQuartier::Marche)
                    {
                        voisinMarche = 1;
                    }
                }
                if(voisinMarche) continue;
                
                nbMarche += q->getZ();

                if (varianteMarche){

                    for(const auto &voisin: q->getVoisins()){

                        if (dynamic_cast<const Place *>(voisin)->getTypePlace() == TypePlace::Marche)
                        {
                            nbMarche += q->getZ();
                        }
                    }
                }
            }
        }
        if (const Place *p = dynamic_cast<const Place *>(hex))
        {
            if (p->getTypePlace() == TypePlace::Marche)
            {
                placeMarche += p->getMultiplicateur();
            }
        }
    }

    return placeMarche + nbMarche;
}

int Plateau::calculerPointsHabitation() const
{
    /*
     *Calcul Le nombre de points donnés par les habitations (en comprenant les places et les varaiantes)
     *
     *@return Le nombre total de points
     */
    int placeHabitation = 0;
    int nbHabitation = 0;

    std::vector<const Quartier *> tabHabitation; // pour stocker toutes les habitations, on détermine le plus grand groupe après

    for (const Hexagone *hex : listeHexagones)
    {
        if (hex->getEstRecouvert())
            continue;

        if (const Quartier *q = dynamic_cast<const Quartier *>(hex))
        {
            if (q->getTypeQuartier() == TypeQuartier::Habitation)
            {
                tabHabitation.push_back(q);
            }
        }
        if (const Place *p = dynamic_cast<const Place *>(hex))
        {
            if (p->getTypePlace() == TypePlace::Habitation)
            {
                placeHabitation += p->getMultiplicateur();
            }
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
                        // On vérifie que le voisin est un quartier (si la conversion marche, p'est le cas, sinon ça renvoie nullprt)

                        if ((quartierVoisin->getTypeQuartier() == TypeQuartier::Habitation) && ContientPas(habVisites, quartierVoisin))
                        {
                            // On vérifie que le quartier voisin n'a pas déjà été visité et que p'est bien une habitation

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

    if (varianteHabitation && nbHabitation >= 10){
        nbHabitation *= 2;
    }

    return placeHabitation * nbHabitation;
}
