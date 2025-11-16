#include "Hexagone.h"

void Hexagone::afficher() const
{
    std::cout << "Hexagone(" << x << "," << y << "," << z << ")"
              << (est_recouvert ? " [recouvert]" : "") << "\n";

    // Affichage du type
    if (this->isQuartier())
    {
        std::cout << "  Quartier : ";
    }
    else if (this->isPlace())
    {
        std::cout << "  Place : ";
    }

    switch (this->getType())
    {
    case TypeHex::Carriere:
        texte_couleur(BLEU);
        std::cout << "  Carrière\n";
        break;
    case TypeHex::Habitation:
    case TypeHex::PHabitation:
        texte_couleur(CYAN);
        std::cout << "Habitation\n";
        break;
    case TypeHex::Marche:
    case TypeHex::PMarche:
        texte_couleur(JAUNE);
        std::cout << "Marché\n";
        break;
    case TypeHex::Temple:
    case TypeHex::PTemple:
        texte_couleur(MAGENTA);
        std::cout << "Temple\n";
        break;
    case TypeHex::Caserne:
    case TypeHex::PCaserne:
        texte_couleur(ROUGE);
        std::cout << "Caserne\n";
        break;
    case TypeHex::Jardin:
    case TypeHex::PJardin:
        texte_couleur(VERT);
        std::cout << "Jardin\n";
        break;
    }
    texte_reset();
}