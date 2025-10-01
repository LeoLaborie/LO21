#include <Plateau.h>
#include <algorithm>
#include <iostream>
#include "Joueur.h"

Plateau::Plateau(Joueur* j) : proprietaire(j) {
    listeTuiles.clear();
    listeHexagones = {
        // Tuile de départ au niveau 0 (z = 0)
        new Place(0, 0, 0, TypePlace::Habitation, 1),
        new Carriere(-1, 1, 0),
        new Carriere(0, -1, 0),
        new Carriere(1, 0, 0)
    };

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
bool ContientPas(const std::vector<T>& v, const T& valeur) {
    return std::find(v.begin(), v.end(), valeur) == v.end();
}

bool Plateau::verifierPlacementTuile(int x,int y ,int z) const {
    std::vector<Tuile*> tuiles_en_dessous;
    bool surElever = false;
    bool toucheParBord = false;

    int dx[6] = {+1, +1, 0, -1, -1, 0}; 
    int dy[6] = {0, -1, -1, 0, +1, +1};

    struct coord{
        int x;
        int y;
        int z;
    };
    coord coords[3];
    coords[0].x=x; //hex0
    coords[0].y=y;
    coords[0].z=z;
    coords[1].x=x-1;  //hex1
    coords[1].y=y+1;
    coords[1].z=z;
    coords[2].x=x;  //hex2
    coords[2].y=y+1;
    coords[2].z=z;
    for (const auto& h : coords) {
        bool supportTrouve = false;

        if (h.z > 1) surElever = true;

        for (auto* hex_plateau : listeHexagones) {
            // pn vérifie si deux tuiles ne se superposent pas
            if (h.x == hex_plateau->getX() &&  h.y == hex_plateau->getY() && h.z == hex_plateau->getZ())
                return false;

            // on récupère les tuiles en dessous si on est à un niveau supérieur à 1
            // et on garde que les tuiles différentes
            if (h.x == hex_plateau->getX() && h.y == hex_plateau->getY() && (h.z - hex_plateau->getZ()) == 1) {
                supportTrouve = true;
                Tuile* parent = hex_plateau->getParent();
                if (ContientPas(tuiles_en_dessous, parent))
                    tuiles_en_dessous.push_back(parent);
            }

            // on vérifie si on touche par le bord une tuile du plateau (si on est au niveau 0)
            if (h.z == 1) {
                for (int i = 0; i < 6; ++i) {
                    if (h.x + dx[i] == hex_plateau->getX() && h.y + dy[i] == hex_plateau->getY() && hex_plateau->getZ() == 0) {
                        toucheParBord = true;
                        break;
                    }
                }
            }
        }

        if (h.z > 1 && !supportTrouve)
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

void Plateau::ajouterTuile(Tuile& t, int x, int y, int z) {
    if (!verifierPlacementTuile(x, y, z)) {
        std::cout << "Placement de tuile invalide." << std::endl;
        return;
    }
    if (z>1){
        std::for_each(listeHexagones.begin(), listeHexagones.end(), [&](Hexagone* h){
        if (h->getX() == x && h->getY() == y  && !h->getEstRecouvert()) {
            h->setEstRecouvert();
            if (dynamic_cast<const Carriere*>(h)){
                proprietaire->setNbrPierres(proprietaire->getNbrPierres()+1);
            }
        }
        });
        std::for_each(listeHexagones.begin(), listeHexagones.end(), [&](Hexagone* h){
        if (h->getX() == x - 1 && h->getY() == y + 1 && !h->getEstRecouvert()){
            h->setEstRecouvert();
            if (dynamic_cast<const Carriere*>(h)){
                proprietaire->setNbrPierres(proprietaire->getNbrPierres()+1);
            }
        }
        });
        std::for_each(listeHexagones.begin(), listeHexagones.end(), [&](Hexagone* h){
        if (h->getX() == x && h->getY() == y + 1 && !h->getEstRecouvert()) {
            h->setEstRecouvert();
            if (dynamic_cast<const Carriere*>(h)){
                proprietaire->setNbrPierres(proprietaire->getNbrPierres()+1);
            }
        }
        });

    }

    auto* h0 = t.getHexagones()[0];
    auto* h1 = t.getHexagones()[1];
    auto* h2 = t.getHexagones()[2];

    h0->SetCoord(x, y, z);
    h1->SetCoord(x - 1, y + 1, z);
    h2->SetCoord(x, y + 1, z);
    // Insérer la tuile dans le plateau
    listeTuiles.push_back(t);
    updateVoisins();
}


int Plateau::calculerPoints() const {



    int placeHabitation=0;
    int placeMarche=0;
    int placeTemple=0;
    int placeCaserne=0;
    int placeJardin=0;

    int nbJardin=0;
    int nbCaserne=0;
    int nbTemple=0;
    int nbHabitation=0;
    int nbMarche=0;

    for (const auto* hex : listeHexagones) {
        if (hex->getEstRecouvert()) continue;

        if (hex->getEstRecouvert()) continue;

        if (auto q = dynamic_cast<const Quartier*>(hex)) {
            if (q->getTypeQuartier()==TypeQuartier::Jardin) nbJardin+=q->getZ();
            
            if (q->getTypeQuartier()==TypeQuartier::Caserne){
                if (q->getVoisins().size()<=3) nbCaserne+=q->getZ(); //si il a 4 voisins ou moins c'est qu'il est sur un bord
            }
            
            if (q->getTypeQuartier()==TypeQuartier::Temple){
                if (q->getVoisins().size()==5) nbTemple+=q->getZ(); 
            }
            if (q->getTypeQuartier()==TypeQuartier::Marche){
                for (const auto& voisin: q->getVoisins()){
                    if (dynamic_cast<const Quartier*>(voisin)->getTypeQuartier()==TypeQuartier::Marche){
                        continue;
                    }
                }
                nbMarche+=q->getZ();

            }

            //reste habitation plus compliqué

        }
        if (auto p = dynamic_cast<const Place*>(hex)) {
            switch (p->getTypePlace()) {
                case TypePlace::Habitation: placeHabitation += p->getMultiplicateur(); break;
                case TypePlace::Marche:     placeMarche     += p->getMultiplicateur(); break;
                case TypePlace::Temple:     placeTemple     += p->getMultiplicateur(); break;
                case TypePlace::Caserne:    placeCaserne    += p->getMultiplicateur(); break;
                case TypePlace::Jardin:     placeJardin     += p->getMultiplicateur(); break;
            }
            continue;
        }
    }
    return placeCaserne*nbCaserne + placeHabitation*nbHabitation + placeJardin*nbJardin + placeMarche*nbMarche + placeTemple*nbTemple;
}