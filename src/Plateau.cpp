#include "Plateau.h"
#include <algorithm>

Plateau::Plateau(bool varianteScore) : varianteScore(varianteScore)
{
    listeTuiles.clear();
    Tuile tuileDepart{new Hexagone(0, 0, 0, TypeHex::PHabitation, 1), new Hexagone(-1, 1, 0, TypeHex::Carriere)
        , new Hexagone(0, -1, 0, TypeHex::Carriere), new Hexagone(1, 0, 0, TypeHex::Carriere)};
    listeTuiles.push_back(tuileDepart);

    updateVoisins();
}

void Plateau::updateVoisins()
{
    /*
    *Met à jour les voisins d'une tuile
    *@return void
    */
    // algo en O(n²), on pouurrait le rendre en O(n), mais vu qu'on a tres peu d'hexagone par plateau le n² n'est pas dérangeant
    // Parcourir toutes les tuiles du plateau
    pourChaqueHexagone([&](Hexagone* hexagone1)
    {
        pourChaqueHexagone([&](Hexagone* hexagone2)
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
        });
    });
}

template <class T>
bool ContientPas(const std::vector<T> &v, const T &valeur)
{
    return std::find(v.begin(), v.end(), valeur) == v.end();
}

bool Plateau::verifierPlacementTuile(const Position &p,const Tuile &t) const
{
    /*
    *Vérifie si une position est correct en fonction d'une tuile (surtout de sa rotation) et de sa nouvelle position théorique
    *@return un bouléen sur la validité de la position passé en paramètre
    */
    std::vector<const Tuile *> tuiles_en_dessous;
    bool surElever = false;
    bool toucheParBord = false;
    int supports_par_hex = 0; //  pour vérifier qu’on pose bien sur 3 hexagones en hauteur

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
    coords.reserve(t.getHexagones().size());
    for (const auto &o : t.getOffsets())
        coords.push_back({p.x + o.q, p.y + o.r, p.z});

    for (const auto &h : coords)
    {
        bool supportTrouve = false;
        bool hexDejaComptePourBord = false; // pour ne compter chaque hex qu'une seule fois
        bool superposition = false;

        if (h.z > 0)
            surElever = true;

        pourChaqueHexagone([&](const Hexagone* hex_plateau)
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
            if (h.z > 0 && h.x == hex_plateau->getX() && h.y == hex_plateau->getY() && (h.z - hex_plateau->getZ()) == 1)
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
                    if (h.x + dx[i] == hex_plateau->getX() &&
                        h.y + dy[i] == hex_plateau->getY())
                    {
                        toucheParBord=true;
                        hexDejaComptePourBord = true;
                        break;
                    }
                }
            }
        });

        if (superposition)
            return false;

        if (h.z > 0 && !supportTrouve)
            return false; // chaque hex doit avoir son support direct
    }

    if (surElever)
    {
        // on pose bien sur 3 hexagones (un support par hex) ET sur au moins 2 tuiles différentes
        if (supports_par_hex != (int)coords.size())
            return false;
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

std::vector<Position> Plateau::getPositionsLegales(const Tuile &t) const{
    /*
    *Calcul toutes les positions légales en fonction d'une tuile (sa rotation)
    *@return un vector contenant toutes les positions corrects
    */
    std::vector<Position> listeValide;
    if (listeTuiles.empty()) return listeValide;


    //on récupere le min et le max
    int minX = listeTuiles[0].getHexagones()[0]->getX();
    int maxX = listeTuiles[0].getHexagones()[0]->getX();
    int minY = listeTuiles[0].getHexagones()[0]->getY();
    int maxY = listeTuiles[0].getHexagones()[0]->getY();
    int maxZ = listeTuiles[0].getHexagones()[0]->getZ();

    pourChaqueHexagone([&](const Hexagone* h)
    {
        minX = std::min(minX, h->getX());
        maxX = std::max(maxX, h->getX());
        minY = std::min(minY, h->getY());
        maxY = std::max(maxY, h->getY());
        maxZ = std::max(maxZ, h->getZ());
    });
    int marge = 3;

    //on se laisse de la marge >1 pour ne pas oublier des positions légales si on prend que 1
    int minXTest = minX - marge;
    int maxXTest = maxX + marge;
    int minYTest = minY - marge;
    int maxYTest = maxY + marge;
    int minZTest = 0;
    int maxZTest = maxZ + 1; 


    //on parcours tt les positions possibles peu d'hexagones donc o(n³) ne pose pas de problème
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
    /*
    *affiche toutes les positions légales possible pour une certaine tuile (surout de sa position en fonction des rotations)
    *@return void
    */
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
    /*
    *Place une tuile dans le plateau si son placement est correct
    *@return Le nombre de carrière recouvert et -1 si placement incorrect 
    */

    int res = 0;

    if (!verifierPlacementTuile(p, t))
    {
        texte_couleur(ROUGE);
        texte_gras_on();
        std::cout << " Placement de tuile invalide." << std::endl;
        texte_reset();
        return -1;
    }

    // positionner la tuile (3 hexagones)
    for (size_t i = 0; i < t.getHexagones().size(); ++i)
    {
        auto *h = t.getHexagones()[i];
        const auto &o = t.getOffsets()[i];
        h->setCoord(p.x + o.q, p.y + o.r, p.z);
    }

    // Insérer la tuile dans le plateau
    listeTuiles.push_back(t);
    updateVoisins();

    // Si on est en hauteur (z > 0), on recouvre ce qui est juste en dessous (z-1)
    if (p.z > 0)
    {
        auto Recouvrir = [&](int x, int y)
        {
            bool traite = false;
            pourChaqueHexagone([&](Hexagone* h)
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
                }
            });
        };

        for (const auto &o : t.getOffsets())
            Recouvrir(p.x + o.q, p.y + o.r);
    }

    return res;
}

int Plateau::calculerPoints() const
{
    /*
    *Calcul le nombre de points total
    *@return Le nombre total de points total
    */
    return calculerPointsCaserne() + calculerPointsHabitation() + calculerPointsJardin() + calculerPointsMarche() + calculerPointsTemple();
}

int Plateau::calculerPointsCaserne() const
{
    /*
    *Calcul le nombre de points donner par les casernes (places et quartiers)
    *@return Le nombre total de points
    */
    int placeCaserne = 0;
    int nbCaserne = 0;

    pourChaqueHexagone([&](const Hexagone *hex)
    {
        if (hex->getEstRecouvert())
            return;

        if (hex->getType() == TypeHex::Caserne)
        {

            if (hex->getVoisins().size() <= 5)
                nbCaserne += hex->getZ(); // si il a 5 voisins ou moins p'est qu'il est sur un bord

            if  (varianteScore && hex->getVoisins().size() <= 3)
                nbCaserne += hex->getZ(); // si la variante est activée et qu'il y a moins de 3 voisins ou moins, on doubles le nb de casernes  
        }
        if(hex->getType() == TypeHex::PCaserne)
                placeCaserne += hex->getMultiplicateur();
    });

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

    pourChaqueHexagone([&](const Hexagone *hex)
    {
        if (hex->getEstRecouvert())
            return;

        if(hex->getType() == TypeHex::Temple){
            if (hex->getVoisins().size() == 6)
                nbTemple += hex->getZ();

            if(varianteScore && hex->getZ() > 1)
                nbTemple += hex->getZ(); // On double les points des temples si ils sont à une hauteur plus hate que 1
            
        }
        if(hex->getType() == TypeHex::PTemple)
                placeTemple += hex->getMultiplicateur();
    });

    return placeTemple * nbTemple;
}

bool Plateau::conditionVarianteJardin (const Hexagone* q) const{
    bool conditionRemplie = 0;

    std::vector<std::vector<int>> coVoisinsRelatifs = {{0, -1}, {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}};
    std::vector<std::vector<int>> coVoisinsVides = {};

    // On trouve les coordonnées relatives vides
    pourChaqueHexagone([&](const Hexagone* h)
    {
        for (const auto &co : coVoisinsRelatifs)
        {
            if ((h->getX() - co[0]) == q->getX() && (h->getY() - co[1]) == q->getY())
            { // Ne pas hésiter à me redemander (Dimitri)
                coVoisinsVides.push_back(co);
            }
        }
    });

    for (const auto &co : coVoisinsVides)
    {
        pourChaqueHexagone([&](const Hexagone* h)
        {
            if ((h->getX() - co[0]) == q->getX() && (h->getY() - co[1]) == q->getY())
            {
                coVoisinsVides.push_back(co);
            }
        });
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

    pourChaqueHexagone([&](const Hexagone *hex)
    {
        if (hex->getEstRecouvert())
            return;

        if(hex->getType() == TypeHex::Jardin){
                nbJardin += hex->getZ();
            
            if(varianteScore && Plateau::conditionVarianteJardin(hex))
                nbJardin += hex->getZ();
        }

        if(hex->getType() == TypeHex::PJardin)
                placeJardin += hex->getMultiplicateur();
    });

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
  
    bool voisinMarche = false;

    pourChaqueHexagone([&](const Hexagone* hex)
    {
        if (hex->getEstRecouvert())
            return;

        if(hex->getType() == TypeHex::Marche){
            voisinMarche = false;
            for (const auto &voisin : hex->getVoisins())
            {
                if(voisin->getType() == TypeHex::Marche)
                {
                    voisinMarche = true;
                    break;
                }
            }
            if(!voisinMarche){
                nbMarche += hex->getZ();

                if (varianteScore){

                    for(const auto &voisin: hex->getVoisins()){

                        if (voisin->getType() == TypeHex::PMarche){
                            nbMarche += hex->getZ();
                            break;
                        }
                    }
                }
            }
        }
        if (hex->getType() == TypeHex::PMarche){
            placeMarche += hex->getMultiplicateur();
        }
    });

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

    std::vector<const Hexagone *> tabHabitation; // pour stocker toutes les habitations, on détermine le plus grand groupe après

    pourChaqueHexagone([&](const Hexagone* hex)
    {
        if (hex->getEstRecouvert())
            return;

        if (hex->getType() == TypeHex::Habitation){
            {
                tabHabitation.push_back(hex);
            }
        }
        if (hex->getType() == TypeHex::PHabitation){
                placeHabitation += hex->getMultiplicateur();
            
        }
    });

    // On détermine le plus grand groupe d'habitations
    std::vector<const Hexagone *> groupeMaxHab; // le groupe d'habitation le plus grand, la taille est à 0 par défaut
    std::vector<const Hexagone *> habVisites;   // liste des habitations déjà visités

    for (const Hexagone *hab : tabHabitation)
    {

        if (ContientPas(habVisites, hab))
        {

            std::vector<const Hexagone *> voisinsHabitation; // les voisins, que l'on va ajouter et supprimer au fur et à mesure
            voisinsHabitation.push_back(hab);

            habVisites.push_back(hab);

            std::vector<const Hexagone *> groupeHabitation; // le groupe d'habitation que l'on est en train de regarder
            groupeHabitation.push_back(hab);

            while (voisinsHabitation.size())
            { // on parcourt les voisins jusqu'à ce qu'il n'y en ait plus

                const Hexagone *habActuelle = voisinsHabitation.back();
                voisinsHabitation.pop_back();

                for (const Hexagone *voisin : habActuelle->getVoisins())
                {

                    if (voisin->getType() == TypeHex::Habitation && ContientPas(habVisites, voisin))
                        {
                            // On vérifie que le quartier voisin n'a pas déjà été visité et que p'est bien une habitation

                            voisinsHabitation.push_back(voisin);
                            groupeHabitation.push_back(voisin);
                            habVisites.push_back(voisin);
                        }
                    
                }
            }

            if (groupeHabitation.size() > groupeMaxHab.size())
            {
                groupeMaxHab = groupeHabitation;
            }
        }
    }

    for (const Hexagone *hab : groupeMaxHab)
    {
        nbHabitation += hab->getZ();
    }

    if (varianteScore && nbHabitation >= 10){
        nbHabitation *= 2;
    }

    return placeHabitation * nbHabitation;
}

std::ostream& operator<<(std::ostream& os, const Plateau& p) 
    {
        /*
        *Surchage l'operator << pour utiliser std::cout
        *@return une référence ostream
        */
        os<< "\nPlateau contient " << p.listeTuiles.size() << " tuiles :\n";
        os<< " ----\n";
        for (const auto &t : p.listeTuiles)
        {
            std::cout << t<<" ----\n";
        }
        return os;
    }
