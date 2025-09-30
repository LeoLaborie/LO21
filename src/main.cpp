#include <iostream>
#include "Plateau.h"
#include "Tuile.h"
#include "Hexagone.h"

int main() {
    Plateau p;

    std::cout << "== Etat initial ==\n";
    std::cout << "Hex init: " << p.getHexagones().size() << "\n";

    // Fabrique une tuile (coords provisoires, seront replacées par ajouterTuile)
    Hexagone* a = new Carriere(0,0,0);
    Hexagone* b = new Carriere(0,0,0);
    Hexagone* c = new Carriere(0,0,0);
    Tuile t(a,b,c);

    // Placement valide : hex0 (haut) en (0,2,0) -> hex1 (gauche) ( -1,3,0 ), hex2 (droite) (0,3,0)
    int x=0,y=2,z=0;
    std::cout << "Pose tuile en ("<<x<<","<<y<<","<<z<<")...\n";
    p.ajouterTuile(t, x, y, z);

    // Placement invalide au même endroit
    std::cout << "Re-tente au même endroit (doit échouer)...\n";
    if (!p.verifierPlacementTuile(x,y,z))
        std::cout << "Refus OK ✅\n";
    else
        std::cout << "ERREUR: Accepté ❌\n";

    // Affiche voisins
    std::cout << "== Voisins ==\n";
    for (auto* h : p.getHexagones()) {
        std::cout << "("<<h->getX()<<","<<h->getY()<<","<<h->getZ()<<") : "
                  << h->getVoisins().size() << " voisin(s)\n";
    }

    return 0;
}
