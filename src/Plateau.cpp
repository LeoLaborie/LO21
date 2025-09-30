#include <Plateau.h>
#include <algorithm>

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
bool ContientPas(const std::vector<T>& v, const T& valeur) {
    return std::find(v.begin(), v.end(), valeur) == v.end();
}

bool Plateau::verifierPlacementTuile(const Tuile &t) const {
    std::vector<Tuile*> tuiles_en_dessous;
    bool surElever = false;
    bool toucheParBord = false;

    int dx[6] = {+1, +1, 0, -1, -1, 0};
    int dy[6] = {0, -1, -1, 0, +1, +1};

    for (auto* hex : t.getHexagones()) {
        bool supportTrouve = false;

        if (hex->getZ() > 0) surElever = true;

        for (auto* hex_plateau : listeHexagones) {
            // pn vérifie si deux tuiles ne se superposent pas
            if (hex->getX() == hex_plateau->getX() && hex->getY() == hex_plateau->getY() && hex->getZ() == hex_plateau->getZ())
                return false;

            // on récupere les tuiles en dessous si on est a un niveau supérieur à 1 et on garde que les tuiles différentes pour pouvoir vérifier la regle genre on pose sur au moins deux tuiles
            if (hex->getX() == hex_plateau->getX() && hex->getY() == hex_plateau->getY() && hex->getZ() - hex_plateau->getZ() == 1) {
                supportTrouve = true;
                Tuile* parent = hex_plateau->getParent();
                if (ContientPas(tuiles_en_dessous, parent))
                    tuiles_en_dessous.push_back(parent);
            }

            //on vérifie si on touche par le bord une tuile du plateau (si on est au niveau 0)
            if (hex->getZ() == 0) {
                for (int i = 0; i < 6; ++i) {
                    if (hex->getX() + dx[i] == hex_plateau->getX() && hex->getY() + dy[i] == hex_plateau->getY() && hex_plateau->getZ() == 0) {
                        toucheParBord = true;
                        break;
                    }
                }
            }
        }

        if (hex->getZ() > 0 && !supportTrouve)
            return false;
    }

    if (surElever) {
        if (tuiles_en_dessous.size() < 2)
            return false;
   } else {
    if (!toucheParBord)
        return false;
}

    return true;
}
